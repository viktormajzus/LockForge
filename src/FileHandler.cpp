#include "FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <filesystem>
#include <openssl/rand.h>
#include "Hash.hpp"
#include "InputManager.hpp"

FileHandler::FileHandler(std::string_view filePath)
{
  this->filePath = std::string(filePath);

  if (!createNewFile())
  {
    throw std::runtime_error("Failed to create new file.");
  }
}

bool FileHandler::createNewFile()
{
  // **Check if the file already exists**
  if (std::filesystem::exists(filePath))
  {
    std::string masterPassword = InputManager::InputPassword("Enter password: ");

    bool isPasswordVerified = verifyPassword(masterPassword);
    masterPassword.clear();

    return isPasswordVerified;
  }

  // **Create the new file**
  std::ofstream file(filePath, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Failed to create new file." << std::endl;
    return false;
  }

  std::string masterPassword = InputManager::InputPassword("Enter password: ");
  std::string confirmPassword = InputManager::InputPassword("Confirm password: ");
  while (masterPassword != confirmPassword)
  {
    std::cout << "Passwords do not match. Please try again.\n";
    masterPassword = InputManager::InputPassword("Enter password: ");
    confirmPassword = InputManager::InputPassword("Confirm password: ");
  }
  confirmPassword.clear();

  // **Write the 13-byte file header**
  file.write("LockForge_1.0", 13);

  salt = Hash::generateSalt(16);

  file.write(reinterpret_cast<const char *>(salt.data()), 16);

  // Convert hashedPassword to string equivalent
  std::string hashedPasswordString{Hash::hashPassword(masterPassword, salt)};
  this->hashedPassword = std::vector<std::byte>(reinterpret_cast<const std::byte *>(hashedPasswordString.data()),
                                                reinterpret_cast<const std::byte *>(hashedPasswordString.data() + hashedPasswordString.size()));

  auto checkValue = Crypt::deriveCheckValue(hashedPasswordString, salt);
  auto [encryptionKey, hmacKey] = Crypt::deriveKeys(checkValue);
  std::vector<std::byte> hmac = Crypt::calculateHMAC(checkValue, hmacKey);

  file.write(reinterpret_cast<const char *>(hmac.data()), 32);

  file.flush();
  file.close();

  std::cout << "Vault file created successfully." << std::endl;
  return true;
}

bool FileHandler::addCredential(std::string_view service, std::string_view username, std::string_view password)
{
  auto [encryptionKey, hmacKey] = Crypt::deriveKeys(hashedPassword);

  // **Pad service name to 64 bytes**
  std::string paddedService = std::string(service);
  paddedService.resize(64, '\0');
  std::vector<std::byte> encryptedService = Crypt::encryptServiceName(paddedService, encryptionKey);

  // **Print encrypted service for debugging**
  std::cout << "Encrypted Service being written: ";
  for (int i = 0; i < 64; i++)
  {
    std::cout << std::to_integer<int>(encryptedService[i]) << " ";
  }
  std::cout << std::endl;

  std::string data = std::string(username) + " " + std::string(password);
  std::vector<std::byte> iv(16);
  std::vector<std::byte> encryptedData = Crypt::encryptCredentials(data, encryptionKey, iv);

  CredentialEntry entry;
  std::memcpy(entry.service, encryptedService.data(), 64);
  std::memcpy(entry.iv, iv.data(), 16);
  entry.encryptedDataSize = static_cast<int>(encryptedData.size());
  std::memset(entry.cipher, 0, 128);
  std::memcpy(entry.cipher, encryptedData.data(), encryptedData.size());

  std::ofstream file(filePath, std::ios::binary | std::ios::app);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file for writing");
  }

  file.write(reinterpret_cast<const char *>(&entry), sizeof(CredentialEntry));

  file.flush();
  file.close();

  return true;
}

bool FileHandler::findCredential(std::string_view service, std::string &username, std::string &password)
{
  auto [encryptionKey, hmacKey] = Crypt::deriveKeys(hashedPassword);
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file." << std::endl;
    return false;
  }

  file.ignore(61); // Header + Salt + CheckValue

  std::string paddedService = std::string(service);
  paddedService.resize(64, '\0');
  std::vector<std::byte> encryptedTargetService = Crypt::encryptServiceName(paddedService, encryptionKey);

  std::cout << "Encrypted Target Service: ";
  for (int i = 0; i < 64; i++)
  {
    std::cout << std::to_integer<int>(encryptedTargetService[i]) << " ";
  }
  std::cout << std::endl;

  CredentialEntry entry;
  while (file.read(reinterpret_cast<char *>(&entry), sizeof(CredentialEntry)))
  {
    if (file.gcount() < sizeof(CredentialEntry))
      break;

    if (std::memcmp(entry.service, encryptedTargetService.data(), 64) == 0)
    {
      std::vector<std::byte> encryptedBlock(entry.cipher, entry.cipher + entry.encryptedDataSize);
      std::vector<std::byte> iv(entry.iv, entry.iv + 16);

      std::string decryptedData = Crypt::decryptData(encryptedBlock, encryptionKey, iv);
      std::istringstream sstream(decryptedData);
      sstream >> username >> password;

      file.close();
      return true;
    }
  }

  file.close();
  return false;
}

bool FileHandler::verifyPassword(std::string_view password)
{
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file.\n";
    return false;
  }

  char header[14] = {};
  file.read(header, 13);
  if (std::string(header).compare("LockForge_1.0") != 0)
  {
    std::cerr << "File corrupted.\n";
    file.close();
    return false;
  }

  std::vector<std::byte> salt(16);
  file.read(reinterpret_cast<char *>(salt.data()), 16);
  if (file.gcount() < 16)
  {
    std::cerr << "File corrupted.\n";
    file.close();
    return false;
  }

  // Print salt in hex
  std::cout << "Salt: ";
  for (auto byte : salt)
  {
    std::cout << std::hex << std::to_integer<int>(byte) << " ";
  }

  std::vector<std::byte> checkValue(32);
  file.read(reinterpret_cast<char *>(checkValue.data()), 32);
  if (file.gcount() < 32)
  {
    std::cerr << "File corrupted.\n";
    file.close();
    return false;
  }

  std::string hashedPasswordString{Hash::hashPassword(password, salt)};

  std::vector<std::byte> derivedCheckValue = Crypt::deriveCheckValue(hashedPasswordString, salt);
  if (derivedCheckValue != checkValue)
  {
    std::cerr << "Password incorrect.\n";
    file.close();
    return false;
  }

  return true;
}