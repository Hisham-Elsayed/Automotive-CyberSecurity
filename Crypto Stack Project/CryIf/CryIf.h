#ifndef CRYIF_H
#define CRYIF_H

#include "Csm_Types.h"

/**
 * @brief Routes a cryptographic job to the configured Crypto driver object.
 *
 * @param channelId Channel used to select the Crypto driver object.
 * @param jobPtr    Job to be processed.
 * @return E_OK when the job was accepted by the driver; otherwise E_NOT_OK.
 */
Std_ReturnType CryIf_ProcessJob(uint32_t channelId, Crypto_JobType *jobPtr);

#endif /* CRYIF_H */