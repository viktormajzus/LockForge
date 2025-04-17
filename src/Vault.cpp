#include <fstream>
#include <filesystem>
#include <expected>

#include <lockforge/Vault.h>
#include <lockforge/platform.hpp>
#include <lockforge/Crypto.hpp>

std::expected<void, Error::Type> Vault::open(std::string masterPassword)
{
  auto defaultPath{ file::getDefaultPath() };
  if (!defaultPath)
    return std::unexpected(defaultPath.error());

  m_filePath = std::move(defaultPath.value());

  if (std::filesystem::exists(m_filePath))
  {
    auto result{ loadFile() };
    if (!result) return result;
  } else
  {
    m_salt = crypto::generateSalt();
    auto result{ saveFile() };
    if (!result) return result;
  }

  auto key{ crypto::deriveKey(masterPassword, m_salt) };
  mem::wipeStr(masterPassword);

  if (!key)
    return std::unexpected(key.error());

  m_key = std::move(key.value());

  return {};
}

std::expected<void, Error::Type> Vault::close()
{
  return std::expected<void, Error::Type>();
}

std::expected<void, Error::Type> Vault::wipe()
{
  return std::expected<void, Error::Type>();
}

std::expected<Credential, Error::Type> Vault::getCredential(std::string_view service)
{
  return std::expected<Credential, Error::Type>();
}

std::expected<void, Error::Type> Vault::addCredential(const Credential& credential)
{
  return std::expected<void, Error::Type>();
}

std::vector<std::string_view> Vault::list() const
{
  return std::vector<std::string_view>();
}

std::expected<void, Error::Type> Vault::removeCredential(std::string_view service)
{
  return std::expected<void, Error::Type>();
}

std::expected<void, Error::Type> Vault::loadFile()
{
  return std::expected<void, Error::Type>();
}

std::expected<void, Error::Type> Vault::saveFile()
{
  return std::expected<void, Error::Type>();
}
