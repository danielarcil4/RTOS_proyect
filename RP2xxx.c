#ifndef __STM32F411xE_H__
#define __STM32F411xE_H__

#include <stdint.h>

/*                        SIO SECTION                                      */
#define SIO_BASE        0xd0000000
#define GPIO_OUT_OFFSET 0x00000010
#define GPIO_OE_OFFSET  0X00000020
#define SIO_GPIO_OUT_XOR_OFFSET 0x0000001C 

#define GPIO_OE               ( *(volatile uint32_t *) (SIO_BASE + GPIO_OE_OFFSET) )
#define GPIO_OUT              ( *(volatile uint32_t *) (SIO_BASE + GPIO_OUT_OFFSET) )
#define SIO_GPIO_OUT_XOR_CTRL ( *(volatile uint32_t *)  (SIO_BASE + SIO_GPIO_OUT_XOR_OFFSET))

/*                       IO_BANK SETION                                    */
#define IO_BANK0_BASE  0x40014000

#define GPIO_CTRL_OFFSET(n) (0x04 + 8*(n))

#define GPIO0_OE   ( (volatile uint32_t *) (SIO_BASE + GPIO_OE_OFFSET + 0x00) )
#define GPIO0_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(0)) )
#define GPIO1_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(1)) )
#define GPIO2_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(2)) )
#define GPIO3_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(3)) )
#define GPIO4_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(4)) )
#define GPIO5_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(5)) )
#define GPIO6_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(6)) )
#define GPIO7_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(7)) )
#define GPIO8_CTRL ( (volatile GPIO_CTRL_TYPE *) (IO_BANK0_BASE + GPIO_CTRL_OFFSET(8)) )

typedef union {
    uint32_t WORD;
    struct{
        uint32_t FUNCSEL : 5; // Function select for the pin
        uint32_t         : 3; // Reserved bits
        uint32_t OUTOVER : 2; // Output override for the pin
        uint32_t         : 2; // Reserved bits
        uint32_t OEOVER  : 2;  // Output enable override for the pin
        uint32_t         : 2; // Reserved bits
        uint32_t INOVER  : 2; // Input override for the pin
        uint32_t         : 10; // Reserved bits
        uint32_t IRQOVER : 2; // IRQ override for the pin
        uint32_t         : 2; // Reserved bits
    } BITS;
} GPIO_CTRL_TYPE;

#define mGPIO_FUNCSEL_SPI     0x00000001U
#define mGPIO_FUNCSEL_UART    0x00000002U
#define mGPIO_FUNCSEL_I2C     0x00000003U
#define mGPIO_FUNCSEL_PWM     0x00000004U
#define mGPIO_FUNCSEL_SIO     0x00000005U
#define mGPIO_FUNCSEL_PIO0    0x00000006U
#define mGPIO_FUNCSEL_PIO1    0x00000007U
#define mGPIO_FUNCSEL_CLOCK   0x00000008U
#define mGPIO_FUNCSEL_USB     0x00000009U

#define mGPIO_OUTOVER         0X00000300U
#define mGPIO_OEOVER          0x00003000U
#define mGPIO_INOVER          0x00030000U
#define mGPIO_IRQOVER         0x30000000U

#define mGPIO_CTRL_RESET      0x00000000U  

/*                            UART SECTION                              */

#define UART0_BASE       0x40034000
#define UART0_DR_OFFSET  0x00000000
#define UART0_FR_OFFSET  0x00000018
#define UARTIBRD_OFFSET  0x00000024
#define UARTFBRD_OFFSET  0x00000028
#define UARTLCR_H_OFFSET 0x0000002C
#define UARTCR_OFFSET    0x00000030

// UART Data Register
#define UART0_DR ( (volatile UARTDR_TYPE *) (UART0_BASE + UART0_DR_OFFSET) )
// UART Flag Register
#define UART0_CR ( (volatile UARTCR_TYPE *) (UART0_BASE + UARTCR_OFFSET) )
// UART Integer Baud Rate Register
#define UART0_IBRD ( (volatile uint32_t *) (UART0_BASE + UARTIBRD_OFFSET) )
// UART Fractional Baud Rate Register
#define UART0_FBRD ( (volatile uint32_t *) (UART0_BASE + UARTFBRD_OFFSET) )
// UART Line Control Register High
#define UART0_LCR_H ( (volatile UARTLCR_H_TYPE *) (UART0_BASE + UARTLCR_H_OFFSET) ) 
// UART Flag Register
#define UART0_FR ( (volatile UARTFR_TYPE *) (UART0_BASE + UART0_FR_OFFSET) ) 

typedef union{
    uint32_t WORD;
    struct {
        uint32_t DATA     :  8; // Data to be transmitted or received
        uint32_t FE       :  1; // Framing error flag
        uint32_t PE       :  1; // Parity error flag
        uint32_t BE       :  1; // Break error flag
        uint32_t OE       :  1; // Overrun error flag
        uint32_t RESERVED : 20; // Reserved bits 
    } BITS;
} UARTDR_TYPE;

typedef union{
    uint32_t WORD;
    struct {
        uint32_t UARTEN      :  1; // UART enable
        uint32_t SIREN       :  1; // SIR enable
        uint32_t SIRLP       :  1; // SIR low power mode
        uint32_t             :  4; // Reserved bit
        uint32_t LBE         :  1; // Loopback enable
        uint32_t TXE         :  1; // Transmit enable
        uint32_t RXE         :  1; // Receive enable
        uint32_t DTR         :  1; // Data terminal ready
        uint32_t RTS         :  1; // Request to send
        uint32_t OUT1        :  1; // Output 1
        uint32_t OUT2        :  1; // Output 2
        uint32_t RTSEN       :  1; // RTS flow control enable
        uint32_t CTSEN       :  1; // CTS flow control enable
        uint32_t RESERVED    :  16; // Reserved bit
    } BITS;
} UARTCR_TYPE;

#define mUARTCR_UARTEN      (1U << 0)
#define mUARTCR_SIREN       (1U << 1)
#define mUARTCR_SIRLP       (1U << 2)
#define mUARTCR_LBE         (1U << 7)
#define mUARTCR_TXE         (1U << 8)
#define mUARTCR_RXE         (1U << 9)
#define mUARTCR_DTR         (1U << 10)
#define mUARTCR_RTS         (1U << 11)
#define mUARTCR_OUT1        (1U << 12)
#define mUARTCR_OUT2        (1U << 13)
#define mUARTCR_RTSEN       (1U << 14)
#define mUARTCR_CTSEN       (1U << 15)
#define mUARTCR_RESET       0x00000000U

typedef union{
    uint32_t WORD;
    struct {
        uint32_t BRK        : 1; // Break control
        uint32_t PEN        : 1; // Parity enable
        uint32_t EPS        : 1; // Even parity select
        uint32_t STP2       : 1; // Two stop bits select
        uint32_t FEN        : 1; // FIFO enable
        uint32_t WLEN       : 2; // Word length select
        uint32_t SPS        : 1; // Stick parity select
        uint32_t RESERVED   : 25; // Reserved bits
    } BITS;
} UARTLCR_H_TYPE;

#define mUARTLCR_H_BRK      (1U << 0)
#define mUARTLCR_H_PEN      (1U << 1)
#define mUARTLCR_H_EPS      (1U << 2)
#define mUARTLCR_H_STP2     (1U << 3)
#define mUARTLCR_H_FEN      (1U << 4)
#define mUARTLCR_H_WLEN_5   0x00000000U  
#define mUARTLCR_H_WLEN_6   (1U << 5)
#define mUARTLCR_H_WLEN_7   (1U << 6)
#define mUARTLCR_H_WLEN_8   (3U << 5)
#define mUARTLCR_H_SPS      (1U << 7)
#define mUARTLCR_H_RESET    0x00000000U

typedef union{
    uint32_t WORD;
    struct {
        uint32_t CTS      :  1; // Clear to send
        uint32_t DSR      :  1; // Data set ready
        uint32_t DCD      :  1; // Data carrier detect
        uint32_t BUSY     :  1; // UART busy
        uint32_t RXFE     :  1; // Receive FIFO empty
        uint32_t TXFF     :  1; // Transmit FIFO full
        uint32_t RXFF     :  1; // Receive FIFO full
        uint32_t TXFE     :  1; // Transmit FIFO empty
        uint32_t RI       :  1; // Ring indicator
        uint32_t RESERVED : 23; // Reserved bits
    } BITS;
} UARTFR_TYPE;

#define mUARTFR_CTS      (1U << 0)
#define mUARTFR_DSR      (1U << 1)
#define mUARTFR_DCD      (1U << 2)
#define mUARTFR_BUSY     (1U << 3)
#define mUARTFR_RXFE     (1U << 4)
#define mUARTFR_TXFF     (1U << 5)
#define mUARTFR_RXFF     (1U << 6)
#define mUARTFR_TXFE     (1U << 7)
#define mUARTFR_RI       (1U << 8)
#define mUARTFR_RESET    0x00000000U

/*                                    CLOCKS SECTION                                              */
#define CLOCKS_BASE 0x40008000
#define CLK_PERI_CTRL_OFFSET 0x00000048

#define CLK_PERI_CTRL ( (volatile CLK_PERI_CTRL_TYPE *) (CLOCKS_BASE + CLK_PERI_CTRL_OFFSET) )

typedef union {
    uint32_t WORD;
    struct {
        uint32_t         : 5; // Reserved bits
        uint32_t AUXSRC  : 3; // AUX clock source
        uint32_t         : 2; // Reserved bit
        uint32_t KILL    : 1; // Asynchronously kills the clock generator
        uint32_t ENABLE  : 1; // clock enable
        uint32_t         : 10; // Reserved bit
    } BITS;
} CLK_PERI_CTRL_TYPE;

#define mCLK_PERI_CTRL_AUXSRC_CLK_SYS               (0U<<0) // AUX clock source is the system clock
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_PLL_SYS        (1U<<5) // AUX clock source is the reference clock
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_PLL_USB        (2U<<5) // AUX clock source is the USB clock
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_ROSC_CLKSRC_PH (3U<<5) // AUX clock source is the ROSC clock
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_XOSC_CLKSRC    (4U<<5) // AUX clock source is the XOSC clock
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_GPIN0          (5U<<5) // AUX clock source is GPIO0
#define mCLK_PERI_CTRL_AUXSRC_CLKSRC_GPIN1          (6U<<5) // AUX clock source is GPIO1
#define mCLK_PERI_CTRL_KILL                         (1U << 10) // Asynchronously kills the clock generator
#define mCLK_PERI_CTRL_ENABLE                       (1U << 11) // Clock enable
#define mCLK_PERI_CTRL_RESET                        0x00000000U // Reset value for the clock control register



#endif // __STM32F411xE_H__