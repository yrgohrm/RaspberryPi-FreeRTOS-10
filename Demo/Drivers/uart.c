#include "uart.h"
#include "gpio.h"

#include <stdint.h>

#define AUX_UART_CLOCK     250000000
#define AUX_UART_BAUD_RATE 115200

#define AUX_ENABLE ((volatile unsigned int *)0x20215004)

typedef struct {
    uint32_t AUX_MU_IO;
    uint32_t AUX_MU_IER;
    uint32_t AUX_MU_IIR;
    uint32_t AUX_MU_LCR;
    uint32_t AUX_MU_MCR;
    uint32_t AUX_MU_LSR;
    uint32_t AUX_MU_MSR;
    uint32_t AUX_MU_SCRATCH;
    uint32_t AUX_MU_CNTL;
    uint32_t AUX_MU_STAT;
    uint32_t AUX_MU_BAUD;
} BCM2835_AUX_UART;

static volatile BCM2835_AUX_UART *const pRegs =
    (BCM2835_AUX_UART *) 0x20215040;

void InitUart()
{
    // set GPIO 14 and 15 to alt5
    SetGpioFunction(14, 2);
    SetGpioFunction(15, 2);
    PudGpio(1 << 14 | 1 << 15, PULL_DISABLE);

    *AUX_ENABLE |= 1;		// enable AUX mini uart   
    pRegs->AUX_MU_IER = 0;	// disable interrupts
    pRegs->AUX_MU_CNTL = 0;	// turn off rx/tx   
    pRegs->AUX_MU_IIR = 0x6;	// clear fifos
    pRegs->AUX_MU_LCR = 3;	// transfer 8bits
    pRegs->AUX_MU_BAUD = AUX_UART_CLOCK / (8 * (AUX_UART_BAUD_RATE + 1));

    pRegs->AUX_MU_CNTL = 3;	// enable TX & RX
}

void PutcUart(char c)
{
    while (!(pRegs->AUX_MU_LSR & 0x20)) {
	asm volatile ("nop");
    }

    pRegs->AUX_MU_IO = (unsigned char) c;
}

char GetcUart()
{
    /* wait for data to arrive */
    while (!(pRegs->AUX_MU_LSR & 0x01)) {
	asm volatile ("nop");
    }

    return (char) (pRegs->AUX_MU_IO);
}

void PutsUart(const char *str)
{
    while (*str) {
	// terminals expect CRLF and not just LF
	if (*str == '\n') {
	    PutcUart('\r');
	}

	PutcUart(*str++);
    }
}
