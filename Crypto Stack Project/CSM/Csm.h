#ifndef CSM_H
#define CSM_H

#include "Csm_Types.h"

/**
 * @brief Calculates a SHA-256 digest.
 *
 * @param inputPtr        Input data.
 * @param inputLength     Input length in bytes.
 * @param resultPtr       Buffer receiving the digest.
 * @param resultLengthPtr Input buffer capacity and output digest length.
 * @return E_OK when the digest is calculated; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_Hash(const uint8_t *inputPtr, uint32_t inputLength, 
                            uint8_t *resultPtr, uint32_t *resultLengthPtr);

/**
 * @brief Stores an encryption key in the Crypto driver.
 *
 * @param keyPtr    Key data.
 * @param keyLength Key length in bytes.
 * @return E_OK when the key is stored; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_SetKey(const uint8_t *keyPtr, uint32_t keyLength);

/**
 * @brief Sets the validity state of the stored encryption key.
 *
 * @param valid Non-zero to mark the key valid, zero to invalidate it.
 * @return E_OK when the state is updated; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_SetKeyValid(uint8_t valid);

/**
 * @brief Encrypts one block using AES-128-ECB.
 *
 * @param inputPtr        Input block.
 * @param inputLength     Input length in bytes.
 * @param resultPtr       Buffer receiving the ciphertext.
 * @param resultLengthPtr Input buffer capacity and output length.
 * @return E_OK when encryption succeeds; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_Encrypt(const uint8_t *inputPtr, uint32_t inputLength,
                            uint8_t *resultPtr, uint32_t *resultLengthPtr);
                            
#endif