#pragma once

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
    Unknown,
    max_errors
  };
}