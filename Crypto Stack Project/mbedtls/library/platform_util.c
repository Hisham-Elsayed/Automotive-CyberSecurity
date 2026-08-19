/*
 * Trimmed-down replacement for mbedTLS's library/platform_util.c.
 *
 * Upstream platform_util.c unconditionally #includes "mbedtls/threading.h",
 * which pulls in a much bigger config surface than this bare-metal project
 * needs. aes.c and sha256.c only actually call mbedtls_platform_zeroize()
 * (to wipe key material / intermediate state from RAM), so that's the only
 * function reproduced here, in the same "don't let the compiler optimize
 * this away" style as upstream.
 */
#include "mbedtls/platform_util.h"
#include <string.h>

/*void mbedtls_platform_zeroize(void *buf, size_t len)
{
    if (len > 0) {
        static void *(*const volatile memset_func)(void *, int, size_t) = memset;
        memset_func(buf, 0, len);
    }
}
*/
void mbedtls_platform_zeroize(void *buf, size_t len)
{
    volatile unsigned char *p = (volatile unsigned char *)buf;
    while (len--) {
        *p++ = 0;
    }
}