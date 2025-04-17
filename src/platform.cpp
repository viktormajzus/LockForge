#include <cstdlib>
#include <filesystem>
#include <expected>

#include <lockforge/platform.hpp>

namespace fs = std::filesystem;

std::expected<std::filesystem::path, Error::Type> file::getDefaultPath()
{
#ifdef LOCKFORGE_PLATFORM_WINDOWS
  const char* p_appdata{ std::getenv("APPDATA") };
  if (!p_appdata)
    return std::unexpected(Error::Type::EnvMissing);
  return fs::path(p_appdata) / "LockForge" / "vault.lf";
#else
  const char* home{ std::getenv("HOME") };
  if (!home)
    return std::unexpected(Error::Type::EnvMissing);
  return fs::path(home) / ".lockforge" / "vault.lf";
#endif
}
