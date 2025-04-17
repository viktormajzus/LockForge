#pragma once
#include <filesystem>
#include <expected>

#include <lockforge/Error.h>

namespace file
{
  std::expected<std::filesystem::path, Error::Type> getDefaultPath();
}