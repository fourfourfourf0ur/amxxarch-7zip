#include "core/job_manager.hpp"

#include "core/logger.hpp"

#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace amxxarch {

namespace fs = std::filesystem;

int JobManager::submit(SubmitRequest request) {
  std::lock_guard lock(jobs_mutex_);

  if (shutting_down_) return 0;

  int job_id = next_job_id_++;
  auto job = std::make_shared<JobState>();
  job->forward_id = request.forward_id;

  ExtractionRequest extraction{
      job_id,
      fs::path(std::move(request.archive_path)),
      fs::path(std::move(request.output_dir)),
      request.format,
      std::move(request.password),
  };

  jobs_.emplace(job_id, job);

  job->worker =
      std::thread([this, job, extraction = std::move(extraction)]() mutable {
        auto sink = [this, forward = job->forward_id](CallbackEvent event) {
          event.forward_id = forward;
          emit(std::move(event));
        };

        try {
          auto backend = make_extractor_backend();
          backend->extract(extraction, job->cancel_requested, sink);
        } catch (const std::exception& error) {
          log_error("job " + std::to_string(extraction.job_id) +
                    " failed: " + error.what());
          sink({extraction.job_id, job->forward_id, ArchiveEventType::FAILED,
                extraction.archive_path.string(), 0, 0, error.what(), true});
        } catch (...) {
          log_error("job " + std::to_string(extraction.job_id) +
                    " failed: unknown C++ exception");
          sink({extraction.job_id, job->forward_id, ArchiveEventType::FAILED,
                extraction.archive_path.string(), 0, 0, "unknown C++ exception",
                true});
        }
      });

  return job_id;
}

bool JobManager::cancel(int job_id) {
  std::lock_guard lock(jobs_mutex_);
  auto it = jobs_.find(job_id);
  if (it == jobs_.end()) return false;

  it->second->cancel_requested = true;
  return true;
}

bool JobManager::is_cancel_requested(int job_id) {
  std::lock_guard lock(jobs_mutex_);
  auto it = jobs_.find(job_id);
  if (it == jobs_.end()) return false;

  return it->second->cancel_requested.load(std::memory_order_relaxed);
}

int JobManager::file_count(int job_id) {
  std::lock_guard lock(jobs_mutex_);
  auto it = jobs_.find(job_id);
  if (it == jobs_.end()) return 0;

  return static_cast<int>(it->second->files.size());
}

bool JobManager::get_file_name(int job_id, int index, std::string& value) {
  return get_file_value(job_id, index, &ExtractedFile::entry, value);
}

bool JobManager::get_file_path(int job_id, int index, std::string& value) {
  return get_file_value(job_id, index, &ExtractedFile::path, value);
}

bool JobManager::get_file_value(int job_id, int index,
                                std::string ExtractedFile::* field,
                                std::string& value) {
  if (index < 0) return false;

  std::lock_guard lock(jobs_mutex_);
  auto it = jobs_.find(job_id);
  if (it == jobs_.end()) return false;

  auto& files = it->second->files;
  if (static_cast<size_t>(index) >= files.size()) return false;

  value = files[static_cast<size_t>(index)].*field;
  return true;
}

std::vector<CallbackEvent> JobManager::drain_events(size_t max_events) {
  return events_.pop_many(max_events);
}

void JobManager::complete_job(int job_id) {
  std::shared_ptr<JobState> job;

  {
    std::lock_guard lock(jobs_mutex_);
    auto it = jobs_.find(job_id);
    if (it == jobs_.end()) return;

    job = std::move(it->second);
    jobs_.erase(it);
  }

  if (job->worker.joinable()) job->worker.join();
}

std::vector<int> JobManager::shutdown() {
  if (shutting_down_.exchange(true)) return std::vector<int>();

  std::vector<std::shared_ptr<JobState>> jobs_to_join;
  std::vector<int> forwards;

  {
    std::lock_guard lock(jobs_mutex_);
    jobs_to_join.reserve(jobs_.size());
    forwards.reserve(jobs_.size());

    for (auto& [job_id, job] : jobs_) {
      job->cancel_requested = true;
      jobs_to_join.push_back(job);
      forwards.push_back(job->forward_id);
    }

    jobs_.clear();
  }

  for (auto& job : jobs_to_join) {
    if (job->worker.joinable()) job->worker.join();
  }

  return forwards;
}

void JobManager::emit(CallbackEvent event) {
  if (event.type == ArchiveEventType::FILE && event.message == "file" &&
      !event.output_path.empty()) {
    std::lock_guard lock(jobs_mutex_);
    auto it = jobs_.find(event.job_id);
    if (it != jobs_.end())
      it->second->files.push_back({event.entry, event.output_path});
  }

  events_.push(std::move(event));
}

JobManager& jobs() {
  static JobManager manager;
  return manager;
}

}  // namespace amxxarch
