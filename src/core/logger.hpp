#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace amxxarch {

using LogFunction = void (*)(const char* message);

void set_log_function(LogFunction function) noexcept;
void log_error(std::string_view message);
std::vector<std::string> drain_log_messages();

}  // namespace amxxarch
