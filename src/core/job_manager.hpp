#pragma once

#include "backend/extractor_backend.hpp"
#include "core/events.hpp"
#include "core/thread_queue.hpp"

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace amxxarch {

struct ExtractedFile {
  std::string entry;
  std::string path;
};

struct SubmitRequest {
  std::string archive_path;
  std::string output_dir;
  ArchiveFormat format = ArchiveFormat::AUTOMATIC;
  int forward_id = 0;
  std::string password;
};

class JobManager {
public:
  int submit(SubmitRequest request);
  bool cancel(int job_id);
  bool is_cancel_requested(int job_id);
  int file_count(int job_id);
  bool get_file_name(int job_id, int index, std::string& value);
  bool get_file_path(int job_id, int index, std::string& value);
  std::vector<CallbackEvent> drain_events(size_t max_events);
  void complete_job(int job_id);
  std::vector<int> shutdown();

private:
  struct JobState {
    int forward_id = 0;
    std::atomic_bool cancel_requested = false;
    std::thread worker;
    std::vector<ExtractedFile> files;
  };

  void emit(CallbackEvent event);
  bool get_file_value(int job_id, int index, std::string ExtractedFile::* field,
                      std::string& value);

  std::atomic_bool shutting_down_ = false;
  std::atomic_int next_job_id_ = 1;
  std::mutex jobs_mutex_;
  std::unordered_map<int, std::shared_ptr<JobState>> jobs_;
  ThreadQueue<CallbackEvent> events_;
};

JobManager& jobs();

}  // namespace amxxarch
