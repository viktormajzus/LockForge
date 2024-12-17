#include "Hash.hpp"

#include <filesystem>
#include <random>

Hash::Hash(std::string_view password, std::string_view path, unsigned int flags)
{
  this->password = password;

  if (!std::filesystem::exists(path) || flags & HASH_FORCE_SALT)
  {
    this->salt = GenerateRandomBytes(16);
  }
}

unsigned int Hash::GetRandomSeed()
{
  std::random_device rd;
  return rd();
}

std::vector<std::byte> Hash::GenerateRandomBytes(std::size_t bytes)
{
  unsigned int seed = GetRandomSeed();
  std::mt19937 generator(seed);

  std::vector<std::byte> randomBytes(bytes);
  std::uniform_int_distribution<int> dist(0, 255);

  for (auto i = 0; i < bytes; ++i)
  {
    randomBytes[i] = static_cast<std::byte>(dist(generator));
  }

  return randomBytes;
}