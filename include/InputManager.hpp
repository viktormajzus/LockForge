#pragma once
#include <string>
#include <string_view>

class InputManager
{
public:
  static std::string InputPassword(std::string_view prompt);
};