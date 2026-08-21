#ifndef CRYPTO_H
#define CRYPTO_H

#include "Csm_Types.h"

/**
 * @brief Stores an encryption key in the software Crypto driver.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param keyPtr         Key data to store.
 * @param keyLength      Key length in bytes.
 * @return E_OK when the key is stored; otherwise E_NOT_OK.
 */
Std_ReturnType Crypto_SetKey(uint32_t driverObjectId, const uint8_t *keyPtr, uint32_t keyLength);

/**
 * @brief Sets the validity state of the stored encryption key.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param valid          Non-zero to mark the key valid, zero to invalidate it.
 * @return E_OK when the state is updated; otherwise E_NOT_OK.
 */
Std_ReturnType Crypto_SetKeyValid(uint32_t driverObjectId, uint8_t valid);

/**
 * @brief Processes a cryptographic job.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param jobPtr         Job to be processed.
 * @return E_OK when processing succeeds; otherwise E_NOT_OK.
 */
Std_ReturnType Crypto_ProcessJob(uint32_t driverObjectId, Crypto_JobType *jobPtr);


#endif