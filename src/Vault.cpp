#include <lockforge/Vault.h>

std::expected<void, Error::Type> Vault::open(std::string_view filePath, std::string_view masterPassword, bool exists)
{
  return std::expected<void, Error::Type>();
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
