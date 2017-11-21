#include <p18f452.h>
#include <float.h>
#include <stdio.h>
#include <delays.h>
#include "flashMemory.h"

#define CEFLASH //
#define CLKSIPO //Clock trigger for the SIPO Shift Registers
#define STROBE //
#define OESIPO //Output enable for the SIPO Shift Registers
#define OEDATA //Output enable for the data Shift Register
#define OEFLASH //Output enable for the Flash Memory
#DEFINE DATASIPO //Pin for the data to the SIPO shift registers
#define SRPL //Parallel load enable for SIPO shift register
#define CP1 //Clock 1 for PISO SR
#define CP2 //Clock 1 for PISO SR
#define DATAPISO //Pin for the data from the SIPO shift registers

void writeByteSR(unsigned int data){
	unsigned int shifts;
	STROBE = 0;
	OESIPO = 0;
	
	for (shifts = 8; shifts > 0; shifts--){
		DATASIPO = data >> (shifts-1);
		CLKSIPO = 0;
		CLKSIPO = 1;
	}
}

unsigned char readSIPO(unsigned int addr){
	unsigned char data = 0;
	int shifts = 0;
	OEDATA = 0;
	SRPL = 0;
	SRPL = 1;
	CP2 = 0;

	for (shifts = 0; shifts < 8; shifts ++){
		CP1 = 1;
		data |= CP1;
		data = data << 1;
		CP1 = 0;
	}
	
	return (data);
}

void writeAddrAndData(unsigned int addr, unsigned int data){
	unsigned int addrHigh = (addr >> 16);
	unsigned int addrMed = (addr >> 8);
	unsigned int addrLow = (addr); 
	
	writeByteSR(data);
	writeByteSR(addrLow);
	writeByteSR(addrMed);
	writeByteSR(addrHig);	
	
}

void writeData(int addr, int data){
	
	
}

void latchData(void){
	
}