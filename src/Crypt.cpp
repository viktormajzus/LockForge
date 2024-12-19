#include "Crypt.hpp"
#include <openssl/evp.h>
#include <stdexcept>
#include <openssl/rand.h>
#include <algorithm>
#include <openssl/hmac.h>

#include <iostream>

#include "Hash.hpp"

std::pair<std::vector<std::byte>, std::vector<std::byte>> Crypt::deriveKeys(const std::vector<std::byte> &hashedPassword)
{
  std::vector<std::byte> encryptionKey(hashedPassword.begin(), hashedPassword.begin() + 32);
  std::vector<std::byte> hmacKey(hashedPassword.begin() + 32, hashedPassword.end());

  return {encryptionKey, hmacKey};
}

std::vector<std::byte> Crypt::calculateHMAC(const std::vector<std::byte> &data, const std::vector<std::byte> &hmacKey)
{
  std::vector<std::byte> hmac(32);
  HMAC(
      EVP_sha256(),
      hmacKey.data(), hmacKey.size(),
      reinterpret_cast<const unsigned char *>(data.data()), data.size(),
      reinterpret_cast<unsigned char *>(hmac.data()), nullptr);

  return hmac;
}

std::vector<std::byte> Crypt::encryptData(std::string_view plaintext, const std::vector<std::byte> &key, std::vector<std::byte> &iv)
{
  if (iv.empty())
  {
    iv.resize(16);
    if (!RAND_bytes(reinterpret_cast<unsigned char *>(iv.data()), iv.size()))
    {
      throw std::runtime_error("Failed to generate IV");
    }
  }

  // ✅ Allocate space for padding
  std::vector<std::byte> encrypted(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
  int encryptedLength{0};
  int finalLength{0};

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
  {
    throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
  }

  // ✅ Initialize AES-256-CBC encryption with key and IV
  if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                          reinterpret_cast<const unsigned char *>(key.data()),
                          reinterpret_cast<const unsigned char *>(iv.data())))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptInit_ex failed");
  }

  // ✅ Encrypt the data
  if (!EVP_EncryptUpdate(ctx,
                         reinterpret_cast<unsigned char *>(encrypted.data()),
                         &encryptedLength,
                         reinterpret_cast<const unsigned char *>(plaintext.data()),
                         plaintext.size()))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptUpdate failed");
  }

  // ✅ Finalize encryption (adds PKCS#7 padding)
  int result = EVP_EncryptFinal_ex(
      ctx,
      reinterpret_cast<unsigned char *>(encrypted.data() + encryptedLength),
      &finalLength);

  if (result != 1)
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptFinal_ex failed");
  }

  // ✅ Print final block to verify padding
  std::cout << "Last 16 bytes of ciphertext: ";
  for (int i = encryptedLength + finalLength - 16; i < encryptedLength + finalLength; i++)
  {
    std::cout << std::to_integer<int>(encrypted[i]) << " ";
  }
  std::cout << std::endl;

  // ✅ Resize encrypted buffer to actual size
  encrypted.resize(encryptedLength + finalLength);
  EVP_CIPHER_CTX_free(ctx);

  return encrypted;
}

std::vector<std::byte> Crypt::encryptData(std::string_view plaintext, const std::vector<std::byte> &key)
{
  std::vector<std::byte> iv(16, std::byte{0});
  std::vector<std::byte> encrypted{};
  encrypted = encryptData(plaintext, key, iv);
  encrypted.resize(64);
  return encrypted;
}

std::string Crypt::decryptData(const std::vector<std::byte> &ciphertext, const std::vector<std::byte> &key, const std::vector<std::byte> &iv)
{
  std::vector<std::byte> decrypted(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
  int decryptedLength{0};
  int finalLength{0};

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
  {
    throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
  }

  // 🔍 Print Key and IV
  std::cout << "Decryption Key: ";
  for (auto byte : key)
    std::cout << std::to_integer<int>(byte) << " ";
  std::cout << "\nDecryption IV: ";
  for (auto byte : iv)
    std::cout << std::to_integer<int>(byte) << " ";
  std::cout << std::endl;

  if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                          reinterpret_cast<const unsigned char *>(key.data()),
                          reinterpret_cast<const unsigned char *>(iv.data())))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_DecryptInit_ex failed");
  }

  if (!EVP_DecryptUpdate(ctx,
                         reinterpret_cast<unsigned char *>(decrypted.data()),
                         &decryptedLength,
                         reinterpret_cast<const unsigned char *>(ciphertext.data()),
                         ciphertext.size()))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_DecryptUpdate failed");
  }

  int result = EVP_DecryptFinal_ex(ctx,
                                   reinterpret_cast<unsigned char *>(decrypted.data() + decryptedLength),
                                   &finalLength);

  // 🔍 Print Decryption Details
  std::cout << "EVP_DecryptFinal_ex result: " << result << std::endl;
  std::cout << "Decrypted Length: " << decryptedLength << ", Final Length: " << finalLength << std::endl;

  if (result != 1)
  {
    EVP_CIPHER_CTX_free(ctx);
    std::cerr << "EVP_DecryptFinal_ex failed: Padding error or Key/IV mismatch" << std::endl;
    throw std::runtime_error("EVP_DecryptFinal_ex failed");
  }

  decrypted.resize(decryptedLength + finalLength);
  EVP_CIPHER_CTX_free(ctx);

  return std::string(reinterpret_cast<const char *>(decrypted.data()), decrypted.size());
}

std::vector<std::byte> Crypt::encryptServiceName(std::string_view serviceName, const std::vector<std::byte> &key)
{
  std::vector<std::byte> iv(16, std::byte{0}); // Fixed IV: 16 bytes of 0
  std::vector<std::byte> encrypted(serviceName.size() + EVP_MAX_BLOCK_LENGTH);
  int encryptedLength{0};
  int finalLength{0};

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
  {
    throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
  }

  if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                          reinterpret_cast<const unsigned char *>(key.data()),
                          reinterpret_cast<const unsigned char *>(iv.data())))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptInit_ex failed");
  }

  if (!EVP_EncryptUpdate(ctx,
                         reinterpret_cast<unsigned char *>(encrypted.data()),
                         &encryptedLength,
                         reinterpret_cast<const unsigned char *>(serviceName.data()),
                         serviceName.size()))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptUpdate failed");
  }

  if (!EVP_EncryptFinal_ex(ctx,
                           reinterpret_cast<unsigned char *>(encrypted.data() + encryptedLength),
                           &finalLength))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptFinal_ex failed");
  }

  encrypted.resize(encryptedLength + finalLength);
  EVP_CIPHER_CTX_free(ctx);

  return encrypted;
}

std::vector<std::byte> Crypt::encryptCredentials(std::string_view plaintext, const std::vector<std::byte> &key, std::vector<std::byte> &iv)
{
  iv.resize(16);
  if (!RAND_bytes(reinterpret_cast<unsigned char *>(iv.data()), iv.size()))
  {
    throw std::runtime_error("Failed to generate IV");
  }

  std::vector<std::byte> encrypted(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
  int encryptedLength{0};
  int finalLength{0};

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx)
  {
    throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
  }

  if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                          reinterpret_cast<const unsigned char *>(key.data()),
                          reinterpret_cast<const unsigned char *>(iv.data())))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptInit_ex failed");
  }

  if (!EVP_EncryptUpdate(ctx,
                         reinterpret_cast<unsigned char *>(encrypted.data()),
                         &encryptedLength,
                         reinterpret_cast<const unsigned char *>(plaintext.data()),
                         plaintext.size()))
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptUpdate failed");
  }

  int result = EVP_EncryptFinal_ex(
      ctx,
      reinterpret_cast<unsigned char *>(encrypted.data() + encryptedLength),
      &finalLength);

  if (result != 1)
  {
    EVP_CIPHER_CTX_free(ctx);
    throw std::runtime_error("EVP_EncryptFinal_ex failed");
  }

  // ✅ Resize buffer to actual size
  encrypted.resize(encryptedLength + finalLength);
  EVP_CIPHER_CTX_free(ctx);

  // ✅ Debug: Print the size of encryptedData
  std::cout << "Size of encryptedData after resize: " << encrypted.size() << std::endl;

  return encrypted;
}

std::vector<std::byte> Crypt::deriveCheckValue(std::string_view password, const std::vector<std::byte> &salt)
{
  Hash hash;

  return hash.simpleHash(password, salt);
}