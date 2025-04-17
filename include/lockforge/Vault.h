#pragma once
#include <cstdint>

#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <filesystem>

#include <expected>

#include <lockforge/Error.h>
#include <lockforge/Credential.h>

class Vault
{
private:
  std::unordered_map<std::string, Credential> m_credentials{};
  std::array<std::uint8_t, 32> m_key{};
  std::array<std::uint8_t, 32> m_salt{};
  std::filesystem::path m_filePath{};

public:
  std::expected<void, Error::Type> open(std::string masterPassword);
  std::expected<void, Error::Type> close();
  std::expected<void, Error::Type> wipe();

  std::expected<Credential, Error::Type> getCredential(std::string_view service);
  std::expected<void, Error::Type> addCredential(const Credential& credential);
  std::vector<std::string_view> list() const;
  std::expected<void, Error::Type> removeCredential(std::string_view service);

private:
  std::expected<void, Error::Type> loadFile();
  std::expected<void, Error::Type> saveFile();
};