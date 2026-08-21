#include "Csm.h"
#include "CryIf.h"
#include "Crypto.h"

/**
 * @brief Calculates a SHA-256 digest through the Crypto stack.
 *
 * @param inputPtr        Input data.
 * @param inputLength     Input length in bytes.
 * @param resultPtr       Buffer receiving the digest.
 * @param resultLengthPtr Input buffer capacity and output digest length.
 * @return E_OK when the digest is calculated; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_Hash(const uint8_t *inputPtr, uint32_t inputLength, 
                            uint8_t *resultPtr, uint32_t *resultLengthPtr)
{
    Crypto_JobType job;
    if (inputPtr == NULL || resultPtr == NULL || resultLengthPtr == NULL) {
        return E_NOT_OK;
    }

    /* Build the job structure - "Single job with ID 0" per the project spec */
    job.jobId          = 0U;
    job.service         = CRYPTO_HASH_GENERATE;
    job.algoFamily      = CRYPTO_ALGOFAM_SHA256;
    job.algoMode        = CRYPTO_ALGOMODE_NOT_SET;
    job.inputPtr        = inputPtr;
    job.inputLength      = inputLength;
    job.outputPtr        = resultPtr;
    job.outputLengthPtr  = resultLengthPtr;

    return CryIf_ProcessJob(CRYIF_CHANNEL_ID_DEFAULT, &job);
}

/**
 * @brief Stores an encryption key in the Crypto driver.
 *
 * @param keyPtr    Key data.
 * @param keyLength Key length in bytes.
 * @return E_OK when the key is stored; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_SetKey(const uint8_t *keyPtr, uint32_t keyLength)
{
    /* Csm_SetKey is a key-management call, not a crypto job in this
     * simplified project, so it goes straight to the driver rather than
     * through a Crypto_JobType - matches the real CSM/CryIf/Crypto SWS,
     * where key management has its own (non-job) API family. */
    return Crypto_SetKey(CRYPTO_DRIVER_OBJECT_ID_SW, keyPtr, keyLength);
}

/**
 * @brief Sets the validity state of the stored encryption key.
 *
 * @param valid Non-zero to mark the key valid, zero to invalidate it.
 * @return E_OK when the state is updated; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_SetKeyValid(uint8_t valid)
{
    return Crypto_SetKeyValid(CRYPTO_DRIVER_OBJECT_ID_SW, valid);
}

/**
 * @brief Encrypts one block using AES-ECB through the Crypto stack.
 *
 * @param inputPtr        Input block.
 * @param inputLength     Input length in bytes.
 * @param resultPtr       Buffer receiving the ciphertext.
 * @param resultLengthPtr Input buffer capacity and output length.
 * @return E_OK when encryption succeeds; otherwise E_NOT_OK.
 */
Std_ReturnType Csm_Encrypt(const uint8_t *inputPtr, uint32_t inputLength,
                            uint8_t *resultPtr, uint32_t *resultLengthPtr)
{
    Crypto_JobType job;

    if (inputPtr == NULL || resultPtr == NULL || resultLengthPtr == NULL) {
        return E_NOT_OK;
    }

    job.jobId          = 0U;
    job.service         = CRYPTO_ENCRYPT;
    job.algoFamily      = CRYPTO_ALGOFAM_AES;
    job.algoMode        = CRYPTO_ALGOMODE_ECB;
    job.inputPtr        = inputPtr;
    job.inputLength      = inputLength;
    job.outputPtr        = resultPtr;
    job.outputLengthPtr  = resultLengthPtr;

    return CryIf_ProcessJob(CRYIF_CHANNEL_ID_DEFAULT, &job);
}