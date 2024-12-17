#pragma once
#include <string>

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
