#pragma once
#include <string>

/// @brief This class is used to determine the path to AppData or its equivalent in other OSes.
class PathHandler
{
private:
  std::string path{};

public:
  PathHandler();

  /// @brief Getter for the path
  /// @return Path to AppData or its equivalent
  std::string GetPath()
  {
    return path;
  }
};
