//
//  commandinterface.h
//  testSTK
//
//  Created by Matthias Röwer on 13.09.12.
//
//

#ifndef testSTK_commandinterface_h
#define testSTK_commandinterface_h

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "uart.h"


#define DEF_COMMANDINTERFACELINELENGTH 32
#define DEF_COMMANDINTERFACECOMMANDS 8
#define DEF_COMMANDINTERFACECOMMANDLENGTH 8
#define DEF_COMMANDINTERFACECOMMANDPARAMETERLENGTH 8

struct commandInterfaceCommand {
	char* name;
	int number;
	void (* retVal)(char* value,void *returnValue);
};



void commandInterfaceInit(void);
int commandInterface(char c, void (* outputFunc)(char c),void *rValue);
int commandInterfaceInterpretLine(char* line,void *rValue);
void commandInterfaceAddCommand(char* name, int number,void (* retVal)(char* value,void *returnValue));

struct commandInterfaceCommand commandInterfaceCommands[DEF_COMMANDINTERFACECOMMANDS];
char commandInterfaceActualLine[DEF_COMMANDINTERFACELINELENGTH];

uint8_t commandInterfaceNumCommands;

uint8_t commandInterfaceActualLineIndex;
uint8_t commandInterfaceActualLineIndexOld;



#endif
