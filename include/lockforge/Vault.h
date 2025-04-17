#pragma once
#include <cstdint>

#include <unordered_map>
#include <vector>
#include <array>
#include <string>
#include <string_view>

#include <expected>

#include <lockforge/Error.h>
#include <lockforge/Credential.h>

class Vault
{
private:
  std::unordered_map<std::string, Credential> credentials{};
  std::string masterKey{};
  std::array<std::uint8_t, 32> salt{};
  std::string filePath{};

public:
  std::expected<void, Error::Type> open(std::string_view filePath, std::string_view masterPassword, bool exists);
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