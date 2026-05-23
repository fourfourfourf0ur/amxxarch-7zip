#pragma once

#include "core/archive_format.hpp"
#include "core/events.hpp"

#include <atomic>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

namespace amxxarch {

namespace fs = std::filesystem;

struct ExtractionRequest {
  int job_id = 0;
  fs::path archive_path;
  fs::path output_dir;
  ArchiveFormat format = ArchiveFormat::AUTOMATIC;
  std::string password;
};

using EventSink = std::function<void(CallbackEvent)>;

class ExtractorBackend {
public:
  virtual ~ExtractorBackend() = default;
  virtual void extract(const ExtractionRequest& request,
                       const std::atomic_bool& cancel,
                       const EventSink& sink) = 0;
};

std::unique_ptr<ExtractorBackend> make_extractor_backend();

}  // namespace amxxarch
