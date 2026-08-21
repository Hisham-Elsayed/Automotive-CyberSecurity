/**
 * @file Csm_Types.h
 * @brief Common types used by the simplified AUTOSAR Crypto stack.
 */
#ifndef CSM_TYPES_H
#define CSM_TYPES_H


#include <stdint.h>
#include <stddef.h>

/** @brief AUTOSAR-style service return type. */
typedef uint8_t  Std_ReturnType;
#define E_OK      0x00U /**< Successful operation. */
#define E_NOT_OK  0x01U /**< Failed operation. */

/** @brief Cryptographic service requested by a job. */
typedef enum
{
    CRYPTO_HASH_GENERATE,
    CRYPTO_ENCRYPT
} Crypto_ServiceType;

/** @brief Cryptographic algorithm family selected by a job. */
typedef enum
{
    CRYPTO_ALGOFAM_SHA256,
    CRYPTO_ALGOFAM_AES
} Crypto_AlgorithmFamilyType;

/** @brief Cryptographic algorithm mode selected by a job. */
typedef enum {
    CRYPTO_ALGOMODE_NOT_SET,
    CRYPTO_ALGOMODE_ECB
} Crypto_AlgorithmModeType;

/**
 * @brief Cryptographic request passed from CSM to the Crypto driver.
 *
 * One job represents one request travelling through CSM, CryIf, and the
 * Crypto driver.
 */
typedef struct 
{
    uint32_t                jobId;
    Crypto_ServiceType      service;
    Crypto_AlgorithmFamilyType      algoFamily;
    Crypto_AlgorithmModeType        algoMode;

    const uint8_t *inputPtr;
    uint32_t      inputLength;

    uint8_t  *outputPtr;
    uint32_t *outputLengthPtr;

} Crypto_JobType;


#define CRYIF_CHANNEL_ID_DEFAULT     0U /**< Default CryIf channel. */
#define CRYPTO_DRIVER_OBJECT_ID_SW   0U /**< Software Crypto driver object. */

#endif