#ifndef CRYPTO_H
#define CRYPTO_H

#include "Csm_Types.h"

Std_ReturnType Crypto_SetKey(uint32_t driverObjectId, const uint8_t *keyPtr, uint32_t keyLength);
Std_ReturnType Crypto_SetKeyValid(uint32_t driverObjectId, uint8_t valid);


Std_ReturnType Crypto_ProcessJob(uint32_t driverObjectId, Crypto_JobType *jobPtr);


#endif