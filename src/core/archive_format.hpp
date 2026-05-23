#pragma once

#include <array>
#include <optional>
#include <string_view>

namespace amxxarch {

enum class ArchiveFormat : int {
  UNKNOWN = -1,
  AUTOMATIC = 0,
  TAR,
  GZIP,
  BZIP2,
  XZ,
  TAR_GZIP,
  TAR_BZIP2,
  TAR_XZ,
  ZIP,
  RAR,
  Z7,
};

ArchiveFormat detect_format_from_name(std::string_view path);
std::optional<ArchiveFormat> parse_format(int value);
std::string_view format_name(ArchiveFormat format) noexcept;
std::array<ArchiveFormat, 10> supported_formats() noexcept;

}  // namespace amxxarch
