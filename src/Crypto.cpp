#include <botan/auto_rng.h>
#include <botan/pbkdf2.h>
#include <botan/hex.h>
#include <botan/hash.h>
#include <botan/mac.h>

#include <random>
#include <lockforge/Crypto.hpp>

std::array<std::uint8_t, crypto::SALT_SIZE> crypto::generateSalt()
{
  std::random_device rd;
  std::uniform_int_distribution<unsigned int> dist(0, 255);

  std::array<std::uint8_t, crypto::SALT_SIZE> salt{};
  for (auto& byte : salt)
    byte = static_cast<std::uint8_t>(dist(rd));

  return salt;
}

std::expected<std::array<std::uint8_t, crypto::KEY_SIZE>, Error::Type> crypto::deriveKey(
  std::string_view password,
  const std::array<std::uint8_t, crypto::SALT_SIZE>& salt)
{
  std::array<std::uint8_t, KEY_SIZE> key{};

  try
  {
    const std::string pwd{ password };
    const std::vector<std::uint8_t> v_salt(salt.begin(), salt.end());

    auto pbkdf{ Botan::PasswordHashFamily::create("PBKDF2") };

    if (!pbkdf)
      return std::unexpected(Error::Type::Unknown);

    auto hasher{ pbkdf->from_iterations(100'000) };

    hasher->derive_key(
      key.data(),
      key.size(),
      pwd.c_str(),
      pwd.size(),
      salt.data(),
      salt.size()
    );
  }
  catch (const Botan::Exception&)
  {
    return std::unexpected(Error::Type::DecryptionFailed);
  }

  return key;
}
