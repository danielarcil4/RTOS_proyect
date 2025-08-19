#include "Inc/led.h"
#include "RP2xxx.c"

#define GPIO2_OUT (1U << 2) // GPIO2 output bit mask

void led_init(void) {
    // Initialization code for the LED
    GPIO2_CTRL->WORD = mGPIO_CTRL_RESET;
    GPIO2_CTRL->BITS.FUNCSEL = mGPIO_FUNCSEL_SIO; // Set function select for GPIO2 to SIO
    GPIO_OE  |= GPIO2_OUT; // Set GPIO2 as output
}

void led_on(void) {
    // Turn on the LED
    GPIO_OUT |= GPIO2_OUT; // Set GPIO2 high
}

void led_off(void) {
    // Turn off the LED
    GPIO_OUT &= ~GPIO2_OUT; // Set GPIO2 low
}

void led_toggle(void) {
    // Toggle the LED state
    SIO_GPIO_OUT_XOR_CTRL |= GPIO2_OUT; // Toggle GPIO2
}