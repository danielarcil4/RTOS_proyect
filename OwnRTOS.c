#include <stdio.h>
#include "pico/stdlib.h"
#include "Inc/led.h"
#include "Inc/uart.h"
#include "Inc/timeBase.h"

int main()
{
    //stdio_init_all();
    led_init();
    init_tx_uart0(); 
    init_timebase();
    while (true) { 
        test();  
    }    
}


