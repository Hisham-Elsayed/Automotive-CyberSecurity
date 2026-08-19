#include "CryIf.h"
#include "Crypto.h"


static Std_ReturnType CryIf_GetDriverObjectId(uint32_t channelId, uint32_t *driverObjectIdPtr)
{
    if(channelId != CRYIF_CHANNEL_ID_DEFAULT)
    {
        return E_NOT_OK;
    }    
    *driverObjectIdPtr = CRYPTO_DRIVER_OBJECT_ID_SW;
    return E_OK;
}

Std_ReturnType CryIf_ProcessJob(uint32_t channelId, Crypto_JobType *jobPtr)
{
    uint32_t driverObjectId;

    if(jobPtr == NULL)
    {
        return E_NOT_OK;
    }

    if(CryIf_GetDriverObjectId(channelId,&driverObjectId) != E_OK)
    {
        return E_NOT_OK;
    }

    return Crypto_ProcessJob(driverObjectId, jobPtr);
}