#ifndef CRYPTO_H
#define CRYPTO_H

#include "lugh.h"

/**
 * Initialize the cryptography subsystem
 */
void crypto_init(void);

/**
 * Generate a cryptographic signature for a message
 * 
 * @param data Pointer to data to sign
 * @param data_len Length of data
 * @param signature Buffer to store signature (must be at least 32 bytes)
 * @param sig_len Size of signature buffer
 * @return true if signature was generated successfully
 */
bool crypto_sign(const void* data, size_t data_len, void* signature, size_t sig_len);

/**
 * Verify a cryptographic signature for a message
 * 
 * @param data Pointer to data to verify
 * @param data_len Length of data
 * @param signature Buffer containing signature 
 * @param sig_len Size of signature
 * @return true if signature is valid
 */
bool crypto_verify(const void* data, size_t data_len, const void* signature, size_t sig_len);

/**
 * Generate a secure hash of data
 * 
 * @param data Pointer to data to hash
 * @param data_len Length of data
 * @param hash Buffer to store hash (must be at least 32 bytes)
 * @param hash_len Size of hash buffer
 * @return true if hash was generated successfully
 */
bool crypto_hash(const void* data, size_t data_len, void* hash, size_t hash_len);

/**
 * Encrypt data using system key
 * 
 * @param data Pointer to plaintext data
 * @param data_len Length of data
 * @param output Buffer to store encrypted data (must be at least data_len + 16 bytes)
 * @param output_len Size of output buffer
 * @param actual_len Actual size of encrypted data
 * @return true if encryption was successful
 */
bool crypto_encrypt(const void* data, size_t data_len, void* output, size_t output_len, size_t* actual_len);

/**
 * Decrypt data using system key
 * 
 * @param data Pointer to encrypted data
 * @param data_len Length of data
 * @param output Buffer to store decrypted data
 * @param output_len Size of output buffer
 * @param actual_len Actual size of decrypted data
 * @return true if decryption was successful
 */
bool crypto_decrypt(const void* data, size_t data_len, void* output, size_t output_len, size_t* actual_len);

/**
 * Compute a SHA-256 hash of the provided data
 * 
 * @param data Pointer to data to hash
 * @param len Length of data in bytes
 * @return 32-bit hash value
 */
uint32_t compute_sha256(const uint8_t *data, size_t len);

#endif /* CRYPTO_H */
