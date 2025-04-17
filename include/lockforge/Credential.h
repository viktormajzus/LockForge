#pragma once
#include <string>
#include <chrono>

struct Credential
{
  std::string service{};
  std::string username{};
  std::string password{};
  std::chrono::year_month_day dateCreated{};
};