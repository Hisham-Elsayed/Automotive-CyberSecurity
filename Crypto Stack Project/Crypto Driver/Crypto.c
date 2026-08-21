#include "Crypto.h"
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include <stddef.h>
#include <string.h>

/* ---- Driver state ------------------------------------------------- */
static uint8_t s_aesKey[32];
static uint32_t s_aesKeyLenBits = 0;
static uint8_t s_aesKeyValid = 0;

/**
 * @brief Stores an AES key for the software Crypto driver.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param keyPtr         Key data to store.
 * @param keyLength      Key length in bytes.
 * @return E_OK when the key is stored; otherwise E_NOT_OK.
 */
Std_ReturnType Crypto_SetKey(uint32_t driverObjectId, const uint8_t *keyPtr, uint32_t keyLength)
{
    (void)driverObjectId;

    if (keyPtr == NULL) {
        return E_NOT_OK;
    }
    if (keyLength > sizeof(s_aesKey)) {
        return E_NOT_OK;
    }

    memcpy(s_aesKey, keyPtr, keyLength);
    s_aesKeyLenBits = keyLength * 8U;

    s_aesKeyValid = 0;

    return E_OK;
}

/**
 * @brief Updates the validity state of the stored AES key.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param valid          Non-zero to mark the key valid, zero otherwise.
 * @return E_OK.
 */
Std_ReturnType Crypto_SetKeyValid(uint32_t driverObjectId, uint8_t valid)
{
    (void)driverObjectId;
    s_aesKeyValid = valid ? 1U : 0U;
    return E_OK;
}

/** @brief Processes a SHA-256 hash-generation job. */
static Std_ReturnType ProcessHashGenerateSha256(Crypto_JobType *jobPtr)
{
    if (jobPtr->outputPtr == NULL || jobPtr->outputLengthPtr == NULL) {
        return E_NOT_OK;
    }
    if (*jobPtr->outputLengthPtr < 32U) {
        return E_NOT_OK; /* SHA-256 digest is always 32 bytes */
    }

    /* mbedtls_sha256(): last arg 0 = SHA-256 (not the SHA-224 variant) */
    mbedtls_sha256(jobPtr->inputPtr, jobPtr->inputLength, jobPtr->outputPtr, 0);

    *jobPtr->outputLengthPtr = 32U;
    return E_OK;
}

/** @brief Processes a single-block AES-ECB encryption job. */
static Std_ReturnType ProcessEncryptAesEcb(Crypto_JobType *jobPtr)
{
    mbedtls_aes_context aesCtx;
    int rc;

    if (!s_aesKeyValid) {
        return E_NOT_OK; /* Csm_SetKey / Csm_SetKeyValid must run first */
    }
    if (jobPtr->outputPtr == NULL || jobPtr->outputLengthPtr == NULL) {
        return E_NOT_OK;
    }
    if (jobPtr->inputLength != 16U) {
        /* Single-block ECB only for this project level - a real driver
         * would loop over 16-byte blocks for longer input. */
        return E_NOT_OK;
    }
    if (*jobPtr->outputLengthPtr < 16U) {
        return E_NOT_OK;
    }

    mbedtls_aes_init(&aesCtx);
    rc = mbedtls_aes_setkey_enc(&aesCtx, s_aesKey, s_aesKeyLenBits);
    if (rc != 0) {
        mbedtls_aes_free(&aesCtx);
        return E_NOT_OK;
    }

    mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_ENCRYPT, jobPtr->inputPtr, jobPtr->outputPtr);
    mbedtls_aes_free(&aesCtx);

    *jobPtr->outputLengthPtr = 16U;
    return E_OK;
}

/**
 * @brief Dispatches a job to the supported cryptographic operation.
 *
 * @param driverObjectId Crypto driver object identifier.
 * @param jobPtr         Job describing the requested operation.
 * @return E_OK when processing succeeds; otherwise E_NOT_OK.
 */
Std_ReturnType Crypto_ProcessJob(uint32_t driverObjectId, Crypto_JobType *jobPtr)
{
    (void)driverObjectId;

    if (jobPtr == NULL) {
        return E_NOT_OK;
    }

    switch (jobPtr->service) {
        case CRYPTO_HASH_GENERATE:
            if (jobPtr->algoFamily != CRYPTO_ALGOFAM_SHA256) {
                return E_NOT_OK; /* only SHA-256 implemented */
            }
            return ProcessHashGenerateSha256(jobPtr);

        case CRYPTO_ENCRYPT:
            if (jobPtr->algoFamily != CRYPTO_ALGOFAM_AES ||
                jobPtr->algoMode   != CRYPTO_ALGOMODE_ECB) {
                return E_NOT_OK; /* only AES-ECB implemented */
            }
            return ProcessEncryptAesEcb(jobPtr);

        default:
            return E_NOT_OK;
    }
}