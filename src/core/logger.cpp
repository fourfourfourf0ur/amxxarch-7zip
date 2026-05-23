#include "core/logger.hpp"

#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace amxxarch {
namespace {

std::mutex log_mutex;
LogFunction log_function = nullptr;
std::vector<std::string> pending_logs;

}  // namespace

void set_log_function(LogFunction function) noexcept {
  std::lock_guard lock(log_mutex);
  log_function = function;
}

void log_error(std::string_view message) {
  std::lock_guard lock(log_mutex);
  pending_logs.emplace_back(message);
}

std::vector<std::string> drain_log_messages() {
  LogFunction log = nullptr;
  std::vector<std::string> messages;

  {
    std::lock_guard lock(log_mutex);
    log = log_function;
    messages.swap(pending_logs);
  }

  if (!log) return {};

  for (const auto& message : messages) log(message.c_str());

  return messages;
}

}  // namespace amxxarch
