#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <cstddef>
#include <utility>

/// @brief This class is used to encrypt and decrypt data using AES-256-GCM and HMAC-SHA-256
class Crypt
{
public:
  static std::pair<std::vector<std::byte>, std::vector<std::byte>> deriveKeys();
  static std::vector<std::byte> calculateHMAC(const std::vector<std::byte> &data, const std::vector<std::byte> &hmacKey);
  static std::vector<std::byte> encryptData(std::string_view plaintext, const std::vector<std::byte> &key, std::vector<std::byte> &iv);
  static std::string decryptData(const std::vector<std::byte> &ciphertext, const std::vector<std::byte> &key, const std::vector<std::byte> &iv);
};