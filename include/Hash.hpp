#pragma once
#include <string>
#include <string_view>
#include <cstddef>
#include <vector>

class Hash
{
public:
  Hash();

  std::string hashPassword(std::string_view password) const;

  bool verifyPassword(const std::string_view hashedPassword, std::string_view password) const;

private:
  std::vector<std::byte> generateSalt(size_t length) const;
};
