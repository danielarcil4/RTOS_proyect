#include "Inc/uart.h"
#include "RP2xxx.c"

#define BAUD_RATE_INTEGER    813 // Integer part of baud rate divisor (for 115200 baud with 48 MHz clock)
#define BAUD_RATE_FRACTION   51 // Fractional part of baud rate divisor
#define GPIO_UART0          (1U<<0)   // GPIO pin for UART0 TX 

static void setup_clocks() {
    // Habilitar el reloj XOSC
    CLK_XOSC_CTRL->WORD    |= kCLK_XOSC_CTRL_FREQ_RANGE_1_15MHZ; // Set frequency range
    CLK_XOSC_STARTUP->WORD |= kCLK_XOSC_STARTUP_DELAY_0_5MS;     // Set startup delay
    CLK_XOSC_CTRL->WORD    |= kCLK_XOSC_CTRL_ENABLE;             // Enable the XOSC
    while(!(CLK_XOSC_STATUS->BITS.STABLE));                      // Wait until XOSC is stable

    // Set the XOSC as source clock for REF, SYS and Periferals
    CLK_REF_CTRL->WORD  |= mCLK_REF_CTRL_SRC_CLKSRC_XOSC_CLKSRC; // Set XOSC as reference clock
    CLK_SYS_CTRL->WORD  |= mCLK_SYS_CTRL_SRC_CLK_REF;            // Set system clock source to CLK_REF
    CLK_REF_DIV->WORD   |= mCLK_REF_DIV_DIVINT;                  // Set CLK_REF divisor to 1
    CLK_PERI_CTRL->WORD |= (mCLK_PERI_CTRL_AUXSRC_CLK_SYS|mCLK_PERI_CTRL_ENABLE); // Set AUX clock source to system clock
}

/* reset the subsystems used in this program */
static void reset_subsystems() {
    // Reset IO_BANK0
    RESETS_CTRL->WORD &= ~mRESETS_RESET_IO_BANK0; // Clear reset bit
    while((RESETS_DONE->WORD & mRESETS_RESET_IO_BANK0) == 0); // Wait until reset is done

    // Reset PADS_BANK0
    RESETS_CTRL->WORD &= ~mRESETS_RESET_PADS_BANK0; // Clear reset bit
    while((RESETS_DONE->WORD & mRESETS_RESET_PADS_BANK0) == 0); // Wait until reset is done

    // Reset UART0
    RESETS_CTRL->WORD &= ~mRESETS_RESET_UART0; // Clear reset bit
    while((RESETS_DONE->WORD & mRESETS_RESET_UART0) == 0); // Wait until reset is done
}

static void config_uart0(){
    *UART0_IBRD = BAUD_RATE_INTEGER; // Set integer part of baud rate divisor
    *UART0_FBRD = BAUD_RATE_FRACTION; // Set fractional
    UART0_LCR_H->WORD = mUARTLCR_H_FEN | mUARTLCR_H_WLEN_8; // Set line control: 8 data bits, no parity, 1 stop bit
    UART0_CR->WORD = mUARTCR_UARTEN | mUARTCR_TXE; // Enable UART and TX
}

void init_tx_uart0(void) {
    // Enable the clock for UART0
    setup_clocks(); 
    // Reset subsystems
    reset_subsystems();
    // Configure UART0
    config_uart0();
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
