#ifndef __UART_H__
#define __UART_H__

void init_tx_uart0(void);
void uart_send(char c);
void uart_send_string(const char* str);

#endif // __UART_H__