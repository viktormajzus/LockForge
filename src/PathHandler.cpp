#define __linux__
#define __APPLE__
#include "PathHandler.hpp"

// OS-Specific Includes
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <cstdlib>
#include <sys/stat.h>
#endif

PathHandler::PathHandler()
{
#if defined(_WIN32)
  char appDataPath[MAX_PATH];
  GetEnvironmentVariableA("APPDATA", appDataPath, MAX_PATH);
  this->path = std::string(appDataPath) + "\\LockForge\\";
  CreateDirectoryA(path.c_str(), NULL);

#elif defined(__linux__)
  const char *xdgDataHome = std::getenv("XDG_DATA_HOME");
  path = xdgDataHome ? std::string(xdgDataHome) + "/LockForge/" : std::string(getenv("HOME")) + "/.local/share/LockForge/";
  mkdir(path.c_str(), 0700);
#elif defined(__APPLE__)
  const char *homePath = std::getenv("HOME");
  path = std::string(homePath) + "/Library/Application Support/LockForge/";
  mkdir(path.c_str(), 0700);
#else
  throw std::runtime_error("Unsupported platform.");
#endif
}