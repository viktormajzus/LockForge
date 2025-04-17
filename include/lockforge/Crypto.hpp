#pragma once
#include <array>
#include <cstdint>
#include <expected>

#include <lockforge/Error.h>

namespace crypto
{
  constexpr std::size_t SALT_SIZE{ 32 };
  constexpr std::size_t KEY_SIZE{ 32 };

  std::array<std::uint8_t, SALT_SIZE> generateSalt();

  std::expected < std::array<std::uint8_t, KEY_SIZE>, Error::Type> deriveKey(
    std::string_view password,
    const std::array<std::uint8_t, SALT_SIZE>& salt
  );
}