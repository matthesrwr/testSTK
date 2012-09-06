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

//#include "uart.c"


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


void commandInterfaceAddCommand(char* name, int number,void (*retVal)(char* value,void *returnValue))
{
	commandInterfaceCommands[commandInterfaceNumCommands].name = name;
	commandInterfaceCommands[commandInterfaceNumCommands].number = number;
	commandInterfaceCommands[commandInterfaceNumCommands].retVal = retVal;
	commandInterfaceNumCommands++;
	if(commandInterfaceNumCommands >= DEF_COMMANDINTERFACECOMMANDS){
		commandInterfaceNumCommands = DEF_COMMANDINTERFACECOMMANDS - 1;
	}
	
}


void commandInterfaceInit(void)
{
	commandInterfaceActualLine[0] = '\0';
	commandInterfaceActualLineIndex = 0;
	commandInterfaceActualLineIndexOld = 0;
	
	commandInterfaceNumCommands = 0;
}

int commandInterface(char c, void (* outputFunc)(char c),void *rValue)
{
	uint8_t returnValue = 0;
	if(c != -1){
		switch (c) {
			case 13:
				outputFunc(10);
				outputFunc(13);
				returnValue = commandInterfaceInterpretLine(commandInterfaceActualLine,rValue);
				commandInterfaceActualLine[0] = '\0';
				commandInterfaceActualLineIndex = 0;
				commandInterfaceActualLineIndexOld = 0;
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
				if (commandInterfaceActualLineIndex >= DEF_COMMANDINTERFACELINELENGTH) {
					commandInterfaceActualLineIndex = DEF_COMMANDINTERFACELINELENGTH - 1;
				}
				commandInterfaceActualLine[commandInterfaceActualLineIndex] = '\0';
				break;
		}
	}
	return returnValue;
}


int commandInterfaceInterpretLine(char* line,void *rValue)
{
	uint8_t i = 0;
	uint8_t j = 0;
	char actLine[DEF_COMMANDINTERFACECOMMANDLENGTH];
	char actCommand[DEF_COMMANDINTERFACECOMMANDPARAMETERLENGTH];
	while (line[i] != '\0' && line[i] != ' ' && i < DEF_COMMANDINTERFACECOMMANDLENGTH) {
		actLine[i] = line[i];
		i++;
	}
	actLine[i] = '\0';
	if(line[i] != '\0'){
		i++;
		while (line[i] != '\0' && i < DEF_COMMANDINTERFACELINELENGTH && j < DEF_COMMANDINTERFACECOMMANDPARAMETERLENGTH) {
			actCommand[j] = line[i];
			i++;
			j++;
			
		}
	}
	actCommand[j] = '\0';
	for(i = 0;i < commandInterfaceNumCommands; i++)
	{
		if (strcmp(commandInterfaceCommands[i].name,actLine) == 0) {
			commandInterfaceCommands[i].retVal(actCommand,rValue);
			
			return commandInterfaceCommands[i].number;
		}
	}
	return 0;
}