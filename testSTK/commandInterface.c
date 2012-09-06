//
//  commandInterface.c
//  testSTK
//
//  Created by Matthias Röwer on 06.09.12.
//
//


#include <avr/io.h>
#include <stdio.h>
#include <string.h>



#define COMMANDINTERFACELINELENGTH 32

void commandInterfaceInit(void);
int commandInterface(char c, void (* outputFunc)(char c));
int commandInterfaceInterpretLine(char* line);

char commandInterfaceActualLine[COMMANDINTERFACELINELENGTH];
uint8_t commandInterfaceActualLineIndex;
uint8_t commandInterfaceActualLineIndexOld;

void commandInterfaceInit(void)
{
	commandInterfaceActualLine[0] = '\0';
	commandInterfaceActualLineIndex = 0;
	commandInterfaceActualLineIndexOld = 0;
}

int commandInterface(char c, void (* outputFunc)(char c))
{
	uint8_t returnValue = 0;
	if(c != -1){
		switch (c) {
			case 13:
				outputFunc(10);
				outputFunc(13);
				returnValue = commandInterfaceInterpretLine(commandInterfaceActualLine);
				commandInterfaceInit();
				break;
			case 127:
				outputFunc(8);
				outputFunc(32);
				outputFunc(8);
				commandInterfaceActualLineIndex = commandInterfaceActualLineIndexOld;
				commandInterfaceActualLine[commandInterfaceActualLineIndex] = '\0';
				break;
			default:
				outputFunc(c);
				commandInterfaceActualLine[commandInterfaceActualLineIndex] = c;
				commandInterfaceActualLineIndexOld = commandInterfaceActualLineIndex;
				commandInterfaceActualLineIndex++;
				if (commandInterfaceActualLineIndex >= COMMANDINTERFACELINELENGTH) {
					commandInterfaceActualLineIndex = COMMANDINTERFACELINELENGTH - 1;
				}
				commandInterfaceActualLine[commandInterfaceActualLineIndex] = '\0';
				break;
		}
	}
	return returnValue;
}


int commandInterfaceInterpretLine(char* line)
{
	if (strcmp(line,"help") == 0) {
		return 1;
	}
	if (strcmp(line,"an") == 0) {
		return 3;
	}
	if (strcmp(line,"aus") == 0) {
		return 2;
	}
	return 0;
}