#include "Inc/uart.h"
#include "RP2xxx.c"

#define BAUD_RATE_INTEGER    813 // Integer part of baud rate divisor (for 115200 baud with 48 MHz clock)
#define BAUD_RATE_FRACTION   51 // Fractional part of baud rate divisor
#define GPIO_UART0          (1U<<0)   // GPIO pin for UART0 TX 

void init_tx_uart0(void) {
    // Enable the clock for UART0
    CLK_PERI_CTRL->WORD |= (mCLK_PERI_CTRL_AUXSRC_CLK_SYS|mCLK_PERI_CTRL_ENABLE); // Set AUX clock source to system clock 
    // Configure UART0
    *UART0_IBRD = BAUD_RATE_INTEGER; // Set integer part of baud rate divisor
    *UART0_FBRD = BAUD_RATE_FRACTION; // Set fractional
    UART0_LCR_H->WORD = mUARTLCR_H_FEN | mUARTLCR_H_WLEN_8; // Set line control: 8 data bits, no parity, 1 stop bit
    UART0_CR->WORD = mUARTCR_UARTEN | mUARTCR_TXE; // Enable UART and TX
    // Set GPIO function to UART0
    GPIO0_CTRL->WORD = mGPIO_FUNCSEL_UART; 
}

void uart_send_string(const char* str) {
    while (*str) {
        uart_send(*str++);
    }
}

void uart_send(char c) {
    // Wait until the UART is ready to transmit
    while ((UART0_FR->WORD & mUARTFR_TXFF) != 0); // Wait until TX FIFO is not full
    // Write the character to the data register
    UART0_DR->WORD = c; // Send character
}
