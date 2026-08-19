/*
 * Minimal mbedTLS config for the AUTOSAR Crypto Stack project.
 *
 * Only the two primitives the project needs are enabled:
 *   - MBEDTLS_SHA256_C : for Csm_Hash (SHA-256)
 *   - MBEDTLS_AES_C    : for Csm_Encrypt (AES-ECB)
 *
 * Everything else (TLS, X.509, RSA, entropy/RNG, the generic cipher
 * wrapper, etc.) is deliberately left OUT. This keeps flash footprint
 * small and avoids pulling in things a bare-metal MCU project doesn't
 * need (like an OS-backed entropy source).
 *
 * If you later add AES-CBC, AES-GCM, HMAC, etc. for the project
 * extension, this is the file you come back to and add the matching
 * MBEDTLS_xxx_C define.
 */
#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#define MBEDTLS_SHA256_C
#define MBEDTLS_AES_C
#define MBEDTLS_AES_ROM_TABLES

/* Suppress "no functionality" style warnings from platform.h for the
 * pieces we didn't enable (e.g. self-test entry points). Not required,
 * but keeps the build log clean. */
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */
