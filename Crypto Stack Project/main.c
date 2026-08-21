#include "TM4C123GH6PM.h"
#include "Csm.h"
#include <string.h>
#include <stdio.h>

/**
 * @file main.c
 * @brief UART-based validation application for the AUTOSAR Crypto stack.
 */

/* ---- UART0 configuration ------------------------------------------ */
#define UART_CTL_UARTEN_BIT   (1U << 0)   /* UARTCTL bit 0: UART enable */
#define UART_CTL_TXE_BIT      (1U << 8)   /* UARTCTL bit 8: transmit enable */
#define UART_CTL_RXE_BIT      (1U << 9)   /* UARTCTL bit 9: receive enable */

#define UART_LCRH_FEN_BIT     (1U << 4)   /* UARTLCRH bit 4: FIFO enable */
#define UART_LCRH_WLEN_8_BITS (0x3U << 5) /* UARTLCRH bits 6:5: word length = 11 (8 bits) */

#define UART_FR_TXFF_BIT      (1U << 5)   /* UARTFR bit 5: transmit FIFO full */

/** @brief Initializes UART0 on PA0 and PA1 at 115200 baud, 8N1. */
static void uart0_init(void)
{
    /* Enable clocks: GPIO port A, UART0 */
    SYSCTL->RCGCGPIO |= (1U << 0);   /* Port A */
    SYSCTL->RCGCUART |= (1U << 0);   /* UART0 */
    __NOP(); __NOP(); __NOP();        /* small delay for clocks to settle */

    /* PA0, PA1 -> UART0 alternate function */
    GPIOA->AFSEL |= (1U << 0) | (1U << 1);
    GPIOA->PCTL   = (GPIOA->PCTL & ~0xFFU) | 0x11U; /* UART on PA0/PA1 = AF1 */
    GPIOA->DEN   |= (1U << 0) | (1U << 1);
    GPIOA->AMSEL &= ~((1U << 0) | (1U << 1));

    /* UART0 config: disable, set baud, 8N1, enable FIFO, re-enable */
    UART0->CTL &= ~UART_CTL_UARTEN_BIT;

    /* Assuming 16 MHz default system clock (no PLL configured).
     * Baud = 16MHz / (16 * 115200) = 8.68 -> IBRD=8, FBRD=round(0.68*64)=44 */
    UART0->IBRD = 27;
    UART0->FBRD = 8;
    UART0->LCRH = UART_LCRH_WLEN_8_BITS | UART_LCRH_FEN_BIT; /* 8 bits, FIFO enabled */
    UART0->CC   = 0; /* system clock as UART clock source */
    UART0->CTL  = UART_CTL_UARTEN_BIT | UART_CTL_TXE_BIT | UART_CTL_RXE_BIT;
}

/** @brief Sends one character through UART0. */
static void uart0_putc(char c)
{
    while (UART0->FR & UART_FR_TXFF_BIT) { /* wait while TX FIFO full */ }
    UART0->DR = (uint8_t)c;
}

/** @brief Sends a null-terminated string through UART0. */
static void uart0_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') uart0_putc('\r'); /* CRLF for terminal programs */
        uart0_putc(*s++);
    }
}

/** @brief Sends a byte buffer as uppercase hexadecimal through UART0. */
static void uart0_print_hex_buf(const uint8_t *buf, uint32_t len)
{
    static const char hexch[] = "0123456789ABCDEF";
    for (uint32_t i = 0; i < len; i++) {
        uart0_putc(hexch[(buf[i] >> 4) & 0xF]);
        uart0_putc(hexch[buf[i] & 0xF]);
    }
}

/* ---- Known-answer test vectors ----------------------------------- */
static const uint8_t g_hashMsg[16] = {
    0x0A,0x27,0x84,0x7C, 0xDC,0x98,0xBD,0x6F,
    0x62,0x22,0x0B,0x04, 0x6E,0xDD,0x76,0x2B
};
static const uint8_t g_hashExpected[32] = {
    0x80,0xC2,0x5E,0xC1, 0x60,0x05,0x87,0xE7, 0xF2,0x8B,0x18,0xB1, 0xB1,0x8E,0x3C,0xDC,
    0x89,0x92,0x8E,0x39, 0xCA,0xB3,0xBC,0x25, 0xE4,0xD4,0xA4,0xC1, 0x39,0xBC,0xED,0xC4
};

/** FIPS-197 Appendix B AES-128-ECB known-answer test vector. */
static const uint8_t g_aesKey[16] = {
    0x2b,0x7e,0x15,0x16, 0x28,0xae,0xd2,0xa6, 0xab,0xf7,0x15,0x88, 0x09,0xcf,0x4f,0x3c
};
static const uint8_t g_aesInput[16] = {
    0x32,0x43,0xf6,0xa8, 0x88,0x5a,0x30,0x8d, 0x31,0x31,0x98,0xa2, 0xe0,0x37,0x07,0x34
};
static const uint8_t g_aesExpected[16] = {
    0x39,0x25,0x84,0x1d, 0x02,0xdc,0x09,0xfb, 0xdc,0x11,0x85,0x97, 0x19,0x6a,0x0b,0x32
};


/**
 * @brief Runs the SHA-256 and AES-ECB known-answer tests.
 *
 * Results are printed over UART0 and execution then remains stopped.
 *
 * @return Does not return.
 */
int main(void)
{
    uint8_t  hashResult[32];
    uint32_t hashLen = sizeof(hashResult);
    uint8_t  ctResult[16];
    uint32_t ctLen = sizeof(ctResult);
    Std_ReturnType rc;

    uart0_init();

    uart0_puts("\nTM4C123 AUTOSAR Crypto Stack Test\n");
    uart0_puts("----------------------------------\n\n");

    /* ---- CSM hash test -------------------------------------------- */
    uart0_puts("Csm_Hash SHA-256 test:\n");
    rc = Csm_Hash(g_hashMsg, sizeof(g_hashMsg), hashResult, &hashLen);
    uart0_puts("  Computed: ");
    uart0_print_hex_buf(hashResult, hashLen);
    uart0_puts("\n  Expected: ");
    uart0_print_hex_buf(g_hashExpected, sizeof(g_hashExpected));
    uart0_puts("\n  Result:   ");
    uart0_puts((rc == E_OK && memcmp(hashResult, g_hashExpected, 32) == 0) ? "PASS" : "FAIL");
    uart0_puts("\n\n");


        /* ---- CSM encryption test -------------------------------------- */
        uart0_puts("Csm_Encrypt AES-ECB test:\n");
    rc = Csm_SetKey(g_aesKey, sizeof(g_aesKey));
    if (rc != E_OK || Csm_SetKeyValid(1U) != E_OK) {
        uart0_puts("AES key setup failed\n");
        while (1) { /* stop after setup failure */ }
    }
    rc = Csm_Encrypt(g_aesInput, sizeof(g_aesInput), ctResult, &ctLen);
    uart0_puts("  Computed: ");
    uart0_print_hex_buf(ctResult, ctLen);
    uart0_puts("\n  Expected: ");
    uart0_print_hex_buf(g_aesExpected, sizeof(g_aesExpected));
    uart0_puts("\n  Result:   ");
    uart0_puts((rc == E_OK && memcmp(ctResult, g_aesExpected, 16) == 0) ? "PASS" : "FAIL");
    // uart0_puts("\n  rc was:   ");
    // uart0_puts(rc == E_OK ? "E_OK" : "E_NOT_OK");
    uart0_puts("\n");
    while (1) { /* done */ }
}