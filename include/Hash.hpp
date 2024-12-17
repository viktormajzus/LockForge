#pragma once
#include <string>
#include <string_view>
#include <cstddef>
#include <vector>

#define HASH_FORCE_SALT 0x01

/// @brief This class is used for password hashing
class Hash
{
private:
  std::string password;
  std::vector<std::byte> salt;
  std::vector<std::byte> key;

public:
  /// @brief The constructor for the hash class
  /// @param password User password
  /// @param path Path of the valut file
  /// @param flags A flag used to force the constructor to behave differently
  Hash(std::string_view password, std::string_view path, unsigned int flags);

private:
  /// @brief A function to generate a random seed
  /// @return An unsigned integer representing the random seed
  unsigned int GetRandomSeed();

  /// @brief A function to generate random bytes
  /// @param bytes The number of bytes to generate
  /// @return Returns a vector of random bytes
  std::vector<std::byte> GenerateRandomBytes(std::size_t bytes);
};
