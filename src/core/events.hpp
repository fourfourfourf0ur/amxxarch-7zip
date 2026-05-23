#pragma once

#include <cstdint>
#include <string>

namespace amxxarch {

enum class ArchiveEventType : int {
  STARTED = 1,
  FILE,
  PROGRESS,
  DONE,
  FAILED,
  CANCELLED,
};

struct CallbackEvent {
  int job_id = 0;
  int forward_id = 0;
  ArchiveEventType type = ArchiveEventType::STARTED;
  std::string entry;
  int64_t processed = 0;
  int64_t total = 0;
  std::string message;
  bool terminal = false;
  std::string output_path;
};

}  // namespace amxxarch
