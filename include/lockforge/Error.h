#pragma once
#include <string_view>

namespace Error
{
  enum class Type
  {
    InvalidPassword,
    FileNotFound,
    FileCorrupted,
    DecryptionFailed,
    JsonParseError,
    ServiceNotFound,
    DuplicateEntry,
    EnvMissing,
    Unknown,
    max_errors
  };

  constexpr std::string_view parse(Type err)
  {
    switch (err)
    {
    case Type::InvalidPassword: return "Invalid master password";
    case Type::FileNotFound: return "Vault file not found";
    case Type::FileCorrupted: return "Vault file is corrupt";
    case Type::DecryptionFailed: return "Vault file failed to decrypt";
    case Type::JsonParseError: return "Failed to parse JSON file";
    case Type::ServiceNotFound: return "Target service not found";
    case Type::DuplicateEntry: return "This entry alread exists";
    case Type::EnvMissing: return "Missing enivornment path";
    case Type::Unknown: return "Unknown error";
    default: return "Unknown error";
    }
  }
}