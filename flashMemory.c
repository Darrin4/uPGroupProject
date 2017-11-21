#include <p18f452.h>
#include <float.h>
#include <stdio.h>
#include <delays.h>
#include "flashMemory.h"

#define CEFLASH
#define CLKSIPO
#define STROBE
#define OESIPO
#define OEFLASH
#DEFINE DATASIPO
#define PL
#define CP1
#define C2
#define DATAPISO

void writeByteSR(char data){
	int shifts;
	STROBE = 0;
	OESIPO = 0;
	for (shifts = 7; shifts > 0; shifts--){
		DATASIPO = data >> shifts;
		CLKSIPO = 1;
		Delay1KTCYx(10);
		CLKSIPO = 1;
	}
}

unsigned char readData(int addr){
	unsigned char data;
	
	return (data);
}

void writeData(int addr, int data){
	
	
}