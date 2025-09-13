#include <stdio.h>
#include "pico/stdlib.h"
#include "Inc/uart.h"
#include "Inc/osKernel.h"

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);
void task0(void);
void task1(void);
void task2(void);
extern void led_init();
extern void led_toggle();

int main()
{
    //stdio_init_all();
    led_init();
    init_tx_uart0();
    init_os_kernel(task0,task1,task2);
    while (true) { 
    }    
}


void task0(void)
{
	while(1)
	{
		motor_run();
        motor_stop();
	}
}


void task1(void)
{
	while(1)
	{
		valve_open();
        valve_close();
	}
}

void task2(void)
{
	while(1)
	{
		led_toggle();
	}
}

void motor_run(void)
{
	uart_send_string("Motor is starting...\n\r");

}

void motor_stop(void)
{
	printf("Motor is stopping...\n\r");
}

void valve_open(void)
{
	uart_send_string("Valve is opening...\n\r");
}


void valve_close(void)
{
	printf("Valve is closing...\n\r");
}
