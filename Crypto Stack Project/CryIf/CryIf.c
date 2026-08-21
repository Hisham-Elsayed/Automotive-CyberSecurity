#include "CryIf.h"
#include "Crypto.h"

/**
 * @brief Maps a CryIf channel to its configured Crypto driver object.
 *
 * @param channelId         Channel to resolve.
 * @param driverObjectIdPtr Pointer receiving the driver object identifier.
 * @return E_OK for a configured channel; otherwise E_NOT_OK.
 */
static Std_ReturnType CryIf_GetDriverObjectId(uint32_t channelId, uint32_t *driverObjectIdPtr)
{
    if (channelId != CRYIF_CHANNEL_ID_DEFAULT) {
        return E_NOT_OK;
    }

    *driverObjectIdPtr = CRYPTO_DRIVER_OBJECT_ID_SW;
    return E_OK;
}

/**
 * @brief Processes one cryptographic job through the Crypto driver.
 *
 * @param channelId Channel used to select the Crypto driver object.
 * @param jobPtr    Job to be processed.
 * @return E_OK when processing succeeds; otherwise E_NOT_OK.
 */
Std_ReturnType CryIf_ProcessJob(uint32_t channelId, Crypto_JobType *jobPtr)
{
    uint32_t driverObjectId;

    if (jobPtr == NULL) {
        return E_NOT_OK;
    }

    if (CryIf_GetDriverObjectId(channelId, &driverObjectId) != E_OK) {
        return E_NOT_OK;
    }

    return Crypto_ProcessJob(driverObjectId, jobPtr);
}