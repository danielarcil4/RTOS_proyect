#include <stdio.h>
#include "pico/stdlib.h"
#include "Inc/led.h"
#include "Inc/uart.h"


int main()
{
    //stdio_init_all();
    led_init();
    init_tx_uart0();
    sleep_ms(1000);
    char c = 'H';
    while (true) {
        uart_send(c);
        led_on();
        sleep_ms(500);
        led_off();
        sleep_ms(500);
    }
}

