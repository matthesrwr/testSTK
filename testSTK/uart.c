//
//  uart.c
//  testSTK
//
//  Created by Matthias Röwer on 05.09.12.
//
//
#include "uart.h"

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