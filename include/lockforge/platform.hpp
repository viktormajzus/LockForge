#pragma once
#include <filesystem>
#include <expected>
#include <string>

#include <lockforge/Error.h>

namespace file
{
  std::expected<std::filesystem::path, Error::Type> getDefaultPath();
}

namespace mem
{
  void wipeStr(std::string& str);
}