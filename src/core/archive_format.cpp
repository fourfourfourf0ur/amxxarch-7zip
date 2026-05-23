#include "core/archive_format.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <optional>
#include <string>
#include <string_view>

namespace amxxarch {
namespace {

struct FormatExtension {
  std::string_view extension;
  ArchiveFormat format;
};

struct FormatName {
  ArchiveFormat format;
  std::string_view name;
};

constexpr std::array FORMAT_EXTENSIONS = {
    FormatExtension{".tar.bz2", ArchiveFormat::TAR_BZIP2},
    FormatExtension{".tar.gz", ArchiveFormat::TAR_GZIP},
    FormatExtension{".tar.xz", ArchiveFormat::TAR_XZ},
    FormatExtension{".bzip2", ArchiveFormat::BZIP2},
    FormatExtension{".gzip", ArchiveFormat::GZIP},
    FormatExtension{".tbz2", ArchiveFormat::TAR_BZIP2},
    FormatExtension{".tar", ArchiveFormat::TAR},
    FormatExtension{".tgz", ArchiveFormat::TAR_GZIP},
    FormatExtension{".tbz", ArchiveFormat::TAR_BZIP2},
    FormatExtension{".txz", ArchiveFormat::TAR_XZ},
    FormatExtension{".bz2", ArchiveFormat::BZIP2},
    FormatExtension{".gz", ArchiveFormat::GZIP},
    FormatExtension{".xz", ArchiveFormat::XZ},
    FormatExtension{".zip", ArchiveFormat::ZIP},
    FormatExtension{".rar", ArchiveFormat::RAR},
    FormatExtension{".7z", ArchiveFormat::Z7}};

constexpr std::array FORMAT_NAMES = {
    FormatName{ArchiveFormat::AUTOMATIC, "auto"},
    FormatName{ArchiveFormat::TAR, "tar"},
    FormatName{ArchiveFormat::GZIP, "gzip"},
    FormatName{ArchiveFormat::BZIP2, "bzip2"},
    FormatName{ArchiveFormat::XZ, "xz"},
    FormatName{ArchiveFormat::TAR_GZIP, "tar.gz"},
    FormatName{ArchiveFormat::TAR_BZIP2, "tar.bz2"},
    FormatName{ArchiveFormat::TAR_XZ, "tar.xz"},
    FormatName{ArchiveFormat::ZIP, "zip"},
    FormatName{ArchiveFormat::RAR, "rar"},
    FormatName{ArchiveFormat::Z7, "7z"},
    FormatName{ArchiveFormat::UNKNOWN, "unknown"}};

std::string lowercase(std::string_view text) {
  std::string result;
  result.reserve(text.size());

  for (unsigned char ch : text)
    result.push_back(static_cast<char>(tolower(ch)));

  return result;
}

bool has_suffix(std::string_view text, std::string_view suffix) {
  return text.size() >= suffix.size() &&
         text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

}  // namespace

std::array<ArchiveFormat, 10> supported_formats() noexcept {
  return {
      ArchiveFormat::TAR,    ArchiveFormat::GZIP,     ArchiveFormat::BZIP2,
      ArchiveFormat::XZ,     ArchiveFormat::TAR_GZIP, ArchiveFormat::TAR_BZIP2,
      ArchiveFormat::TAR_XZ, ArchiveFormat::ZIP,      ArchiveFormat::RAR,
      ArchiveFormat::Z7,
  };
}

std::optional<ArchiveFormat> parse_format(int value) {
  auto format = static_cast<ArchiveFormat>(value);
  if (format == ArchiveFormat::AUTOMATIC) return format;

  auto formats = supported_formats();
  if (std::find(formats.begin(), formats.end(), format) == formats.end())
    return std::nullopt;

  return format;
}

ArchiveFormat detect_format_from_name(std::string_view path) {
  std::string name = lowercase(path);

  for (const auto& item : FORMAT_EXTENSIONS) {
    if (has_suffix(name, item.extension)) return item.format;
  }

  return ArchiveFormat::UNKNOWN;
}

std::string_view format_name(ArchiveFormat format) noexcept {
  for (const auto& item : FORMAT_NAMES) {
    if (item.format == format) return item.name;
  }

  return "unknown";
}

}  // namespace amxxarch
