#include "Hash.hpp"
#include <argon2.h>
#include <stdexcept>
#include <random>

Hash::Hash()
{
}

std::string Hash::hashPassword(std::string_view password, const std::vector<std::byte> &salt)
{
  const int hashLength{32};
  const int encodedLength{128};
  std::vector<std::byte> hashEncoded(encodedLength);

  const uint32_t timeCost{3};
  const uint32_t memoryCost{65536};
  const uint32_t parallelism{1};

  int result{};
  result = argon2i_hash_encoded(
      timeCost, memoryCost, parallelism,
      password.data(), password.size(),
      reinterpret_cast<const char *>(salt.data()), salt.size(),
      hashLength, reinterpret_cast<char *>(hashEncoded.data()),
      hashEncoded.size());

  if (result != ARGON2_OK)
  {
    throw std::runtime_error("Failed to hash password");
  }

  return std::string(reinterpret_cast<const char *>(hashEncoded.data()), hashEncoded.size());
}

bool Hash::verifyPassword(const std::string_view hashedPassword, std::string_view password) const
{
  int result{argon2i_verify(
      hashedPassword.data(),
      password.data(),
      password.size())};

  return result == ARGON2_OK;
}

std::vector<std::byte> Hash::generateSalt(size_t length)
{
  std::vector<std::byte> salt(length);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 255);

  for (auto i = 0; i < length; ++i)
  {
    salt[i] = static_cast<std::byte>(distrib(gen));
  }

  return salt;
}

std::vector<std::byte> Hash::simpleHash(std::string_view plaintext, const std::vector<std::byte> &salt) const
{
  std::vector<std::byte> hash(32);
  argon2i_hash_raw(3, 65536, 1, plaintext.data(), plaintext.size(), salt.data(), salt.size(), hash.data(), hash.size());
  return hash;
}