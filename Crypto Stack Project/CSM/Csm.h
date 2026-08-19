#ifndef CSM_H
#define CSM_H

#include "Csm_Types.h"

Std_ReturnType Csm_Hash(const uint8_t *inputPtr, uint32_t inputLength, 
                            uint8_t *resultPtr, uint32_t *resultLengthPtr);


Std_ReturnType Csm_SetKey(const uint8_t *keyPtr, uint32_t keyLength);
Std_ReturnType Csm_SetKeyValid(uint8_t valid);

Std_ReturnType Csm_Encrypt(const uint8_t *inputPtr, uint32_t inputLength,
                            uint8_t *resultPtr, uint32_t *resultLengthPtr);
                            
#endif