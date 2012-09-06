/*
 *  main.c
 *  testSTK
 *
 *  Created by Matthias Röwer on 01.07.12.
 *  Copyright __MyCompanyName__ 2012. All rights reserved.
 *
 */
#define PORT_OFF(port,pin) port &= ~(1<<pin)
#define PORT_ON(port,pin) port |= 1<<pin
#define PORT_IS_ON(port,pin) (port & (1<<pin))
#define PORT_IS_OFF(port,pin) (!(port & (1<<pin)))

#include <avr/io.h>
#include <stdio.h>
#include "uart.c"
#include "commandInterface.c"

#ifndef F_CPU
#define F_CPU 3686400
#endif

void returnInt(char* value,void *returnValue);

int main(void)
{

	stdout = &mystd;
	stdin = &mystd;
	uart_init();
	void *commandInterfaceRetVal;
	double tmp = 0.0;
	commandInterfaceRetVal = &tmp;
	uint16_t *commandInterfaceRetValuint16_t;
	commandInterfaceRetValuint16_t = commandInterfaceRetVal;
	PORT_ON(DDRB,PB0); // PB0 as output
	PORT_ON(DDRB,PB1); // PB1 as output
	PORT_ON(DDRB,PB2); // PB2 as output
	PORT_ON(DDRB,PB3); // PB3 as output
	PORT_ON(DDRB,PB4); // PB4 as output
	PORT_ON(DDRB,PB5); // PB5 as output
	PORTB = 0xFF; // all ports high, LED off
	
	PORT_OFF(DDRC,PC2); // PC2 as input
	PORT_OFF(DDRC,PC3); // PC3 as input
	
	sei();
	commandInterfaceInit();
	commandInterfaceAddCommand("an",3,&returnInt);
	commandInterfaceAddCommand("aus",2,&returnInt);
	printf("Hallo Welt\nHier ist ihr Controller\n");
	// Main loop
	while(1)
	{
		switch (commandInterface(uart_getc(),&uart_putc,commandInterfaceRetVal)) {
			case 1:
				printf("an: Schaltet das Licht an\naus schaltet das Licht aus\n");
				break;
			case 2:
				PORTB = 0xff;
				printf("licht ist aus:%u\n",*commandInterfaceRetValuint16_t);
				break;
			case 3:
				PORTB = 0x00;
				uint16_t *tmp;
				tmp = commandInterfaceRetVal;
				printf("licht ist an:%u\n",*commandInterfaceRetValuint16_t);
				break;				
			default:
				break;
		}
	}
	return 0;
}

void returnInt(char* value,void *returnValue){
	uint16_t *tmp = returnValue;
	*tmp = 0;
	uint8_t i = 0;
	while (value[i] != '\0') {
		*tmp = (*tmp * 10) + (value[i] - 48);
		i++;
	}
}







