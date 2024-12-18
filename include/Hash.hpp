#pragma once
#include <string>
#include <string_view>
#include <cstddef>
#include <vector>

/// @brief This class is used to hash and verify passwords using the Argon2i algorithm.
class Hash
{
public:
  /// @brief Default constructor - Unused
  Hash();

  /// @brief A function that hashed the password
  /// @param password A string_view containing the password to hash, no max length for now
  /// @return Returns the hashed password as a string
  std::string hashPassword(std::string_view password) const;

  /// @brief A function used to verify whether the hashed password and the password match
  /// @param hashedPassword Hashed password
  /// @param password Plain text password
  /// @return True if the passwords match, false otherwise
  bool verifyPassword(const std::string_view hashedPassword, std::string_view password) const;

private:
  /// @brief Generates a random salt of size length
  /// @param length Salt size in bytes
  /// @return A vector of bytes containing the salt
  std::vector<std::byte> generateSalt(size_t length) const;
};
