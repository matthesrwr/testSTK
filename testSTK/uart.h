//
//  uart.h
//  testSTK
//
//  Created by Matthias Röwer on 13.09.12.
//
//

#ifndef testSTK_uart_h
#define testSTK_uart_h



#define PORT_OFF(port,pin) port &= ~(1<<pin)
#define PORT_ON(port,pin) port |= 1<<pin
#define PORT_IS_ON(port,pin) (port & (1<<pin))
#define PORT_IS_OFF(port,pin) (!(port & (1<<pin)))


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 3686400
#endif




#define UART_BAUD_RATE 115200
#define UART_TX_BUFFER_SIZE 128
#define UART_RX_BUFFER_SIZE 128

void uart_init(void);
int uart_putchar(char c, FILE *stream);
void uart_putc(char c);

char uart_getc();


volatile char uart_tx_buffer[UART_TX_BUFFER_SIZE];
volatile int uart_tx_bufferWriteIndex;
volatile int uart_tx_bufferReadIndex;

volatile char uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile int uart_rx_bufferWriteIndex;
volatile int uart_rx_bufferReadIndex;






#endif
