#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <cstddef>
#include "Crypt.hpp"

struct CredentialEntry
{
  std::byte service[64]; // 64 bytes for encrypted service name
  std::byte iv[16];      // 16 bytes for IV
  int encryptedDataSize; // 4 bytes for size of encrypted credentials
  std::byte cipher[128]; // Max 128 bytes, but we only use encryptedDataSize
};

/// @brief This class is used to handle file operations
/// @details This class is used to create, read, write, and delete files. It works together with the Crypt class to encrypt and decrypt data
class FileHandler
{
private:
  std::string filePath;
  std::vector<std::byte> hashedPassword;
  std::vector<std::byte> salt;

public:
  FileHandler(std::string_view filePath);

  bool createNewFile();
  bool addCredential(std::string_view service, std::string_view username, std::string_view password);
  bool removeCredential(std::string_view service);
  bool findCredential(std::string_view service, std::string &username, std::string &password);
  bool verifyPassword(std::string_view password);
};