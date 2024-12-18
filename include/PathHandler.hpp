#pragma once
#include <string>

/// @brief This class is used to determine the path to AppData or its equivalent in other OSes.
class PathHandler
{
private:
  std::string path{};

public:
  PathHandler();

  std::string GetPath()
  {
    return path;
  }
};
