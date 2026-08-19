#ifndef CSM_TYPES_H
#define CSM_TYPES_H


#include <stdint.h>
#include <stddef.h>

/* ---- Basic AUTOSAR-style typedefs -------------------------------- */
typedef uint8_t  Std_ReturnType;
#define E_OK      0x00U
#define E_NOT_OK  0x01U

/* ---- Service / algorithm identification --------------------------- */
typedef enum
{
    CRYPTO_HASH_GENERATE,
    CRYPTO_ENCRYPT
} Crypto_ServiceType;

typedef enum
{
    CRYPTO_ALGOFAM_SHA256,
    CRYPTO_ALGOFAM_AES
} Crypto_AlgorithmFamilyType;

typedef enum {
    CRYPTO_ALGOMODE_NOT_SET,
    CRYPTO_ALGOMODE_ECB
} Crypto_AlgorithmModeType;

/* ---- Job structure -------------------------------------------------
 * One job = one request travelling down through CSM -> CryIf -> Crypto
 * driver. */

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


#define CRYIF_CHANNEL_ID_DEFAULT     0U
#define CRYPTO_DRIVER_OBJECT_ID_SW   0U

#endif