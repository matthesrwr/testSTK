//
//  uart.c
//  testSTK
//
//  Created by Matthias Röwer on 05.09.12.
//
//
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

static FILE mystd = FDEV_SETUP_STREAM( uart_putchar, NULL, _FDEV_SETUP_WRITE );

volatile char uart_tx_buffer[UART_TX_BUFFER_SIZE];
volatile int uart_tx_bufferWriteIndex;
volatile int uart_tx_bufferReadIndex;

volatile char uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile int uart_rx_bufferWriteIndex;
volatile int uart_rx_bufferReadIndex;

ISR(USART_UDRE_vect)
{
	if(uart_tx_bufferReadIndex == uart_tx_bufferWriteIndex){
		PORT_OFF(UCSRB,UDRIE);
	}else{
		UDR = uart_tx_buffer[uart_tx_bufferReadIndex];
		uart_tx_bufferReadIndex++;
		if(uart_tx_bufferReadIndex >= UART_TX_BUFFER_SIZE){
			uart_tx_bufferReadIndex = 0;
		}
	}
}

ISR(USART_RXC_vect)
{
	uart_rx_buffer[uart_rx_bufferWriteIndex] = UDR;
	uart_rx_bufferWriteIndex++;
	if(uart_rx_bufferWriteIndex >= UART_RX_BUFFER_SIZE){
		uart_rx_bufferWriteIndex = 0;
	}
	
}

void uart_putc(char c)
{
	uart_tx_buffer[uart_tx_bufferWriteIndex] = c;
	uart_tx_bufferWriteIndex++;
	if(uart_tx_bufferWriteIndex >= UART_TX_BUFFER_SIZE){
		uart_tx_bufferWriteIndex = 0;
	}
	PORT_ON(UCSRB,UDRIE);
	
}

int uart_putchar( char c, FILE *stream )
{
    if( c == '\n' )
		uart_putchar( '\r', stream );
	uart_putc(c);
	return 0;
}

char uart_getc(){
	if(uart_rx_bufferReadIndex == uart_rx_bufferWriteIndex){
		return -1;
	}else{
		char c = uart_rx_buffer[uart_rx_bufferReadIndex];
		uart_rx_bufferReadIndex++;
		if(uart_rx_bufferReadIndex >= UART_RX_BUFFER_SIZE){
			uart_rx_bufferReadIndex = 0;
		}
		return c;
	}
}

void uart_init(void)
{
	uart_tx_buffer[0] = '\0';
	uart_tx_bufferReadIndex = 0;
	uart_tx_bufferWriteIndex = 0;
	
	
    UCSRB |= (1<<TXEN) | (1<<RXEN);    // UART TX und RX einschalten
	UCSRB |= (1<<RXCIE);
    UCSRC |= (1<<URSEL)| (1<<UCSZ0) | (1<<UCSZ1);    // Asynchron 8N1
	
    UBRRH = 0;
    UBRRL = 23;
	

}