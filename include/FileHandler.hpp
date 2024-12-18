#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <cstddef>
#include "Crypt.hpp"

/// @brief This class is used to handle file operations
/// @details This class is used to create, read, write, and delete files. It works together with the Crypt class to encrypt and decrypt data
class FileHandler
{
private:
  std::string filePath;
  std::vector<std::byte> hashedPassword;

public:
  FileHandler(std::string_view filePath, std::string_view hashedPassword);

  bool createNewFile();
  bool addCredential(std::string_view service, std::string_view username, std::string_view password);
  bool removeCredential(std::string_view service);
  bool findCredential(std::string_view service, std::string &username, std::string &password);
};