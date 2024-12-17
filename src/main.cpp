#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <print>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>
#endif

#include "PathHandler.hpp"

std::string InputPassword(std::string_view prompt)
{
  std::string password;
  std::print("{}", prompt);

#if defined(_WIN32)
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hStdin, &mode);
  SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
  std::cin >> password;
  SetConsoleMode(hStdin, mode);

#elif defined(__linux__) || defined(__APPLE__)
  termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  std::cin >> password;
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

  return password;
}

int main()
{
  PathHandler pathHandler = PathHandler();
  std::string path = pathHandler.GetPath();

  std::string password = InputPassword("Enter password: ");
  std::string confirmPassword = InputPassword("Confirm password: ");
  while (password != confirmPassword)
  {
    std::print("Passwords do not match. Please try again.\n");
    password = InputPassword("Enter password: ");
    confirmPassword = InputPassword("Confirm password: ");
  }

  return 0;
}