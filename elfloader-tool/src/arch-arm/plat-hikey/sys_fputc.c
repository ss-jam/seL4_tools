/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(NICTA_GPL)
 */

#include "../stdint.h"
#include "../stdio.h"
#include "platform.h"

#define UARTDR      0x000
#define UARTFR      0x018
#define UARTFR_TXFF (1 << 5)

#define UART_REG(x) ((volatile uint32_t *)(UART_PPTR + (x)))

int
__fputc(int c, FILE *stream)
{
    /* Wait until UART ready for the next character. */
    while ((*UART_REG(UARTFR) & UARTFR_TXFF) != 0);

    /* Add character to the buffer. */
    *UART_REG(UARTDR) = (c & 0xff);

    /* Send '\r' after every '\n'. */
    if (c == '\n') {
        (void)__fputc('\r', stream);
    }

    return 0;
}