#include <p18f452.h>
#include <float.h>
#include <stdio.h>
#include <delays.h>
#include "flashMemory.h"


extern void writeByteSR(unsigned int data){
	unsigned int shifts;
	STROBE = 0;
	OESIPO = 0;
	OEDATA = 0;
	
	for (shifts = 8; shifts > 0; shifts--){
		DATASIPO = (data >> (8 - shifts))&1;
		CLKSIPO = 0;
		CLKSIPO = 1;
	}
}

extern unsigned char readBytePISO(){
	unsigned char data = 0;
	int shifts = 0;
	OEDATA = 0;
	SRPL = 0;
	SRPL = 1;
	CP2 = 0;
	CP1 = 0;
	for (shifts = 0; shifts < 8; shifts ++){
		CP1 = 1;
		data |= DATAPISO;
		data = data << 1;
		CP1 = 0;
	}
	
	return (data);
}

extern unsigned char readFlash(unsigned int addr){
	unsigned char data;
	
	writeAddrSIPO(addr);
	latchAddr();
	data = readBytePISO();
	
	return (data);
}

extern void writeAddrAndData(unsigned int addr, unsigned char data){
	unsigned int addrHigh = (addr >> 16);
	unsigned int addrMed = (addr >> 8);
	unsigned int addrLow = (addr); 
	
	writeByteSR((unsigned int)data);
	writeByteSR(addrLow);
	writeByteSR(addrMed);
	writeByteSR(addrHigh);	
	latchAddrAndData();
	
}

extern void writeAddrSIPO(unsigned int addr){
	unsigned int addrHigh = (addr >> 16);
	unsigned int addrMed = (addr >> 8);
	unsigned int addrLow = (addr); 
	
	OEDATA = 0;
	OESIPO = 0;
	
	writeByteSR(addrLow);
	writeByteSR(addrMed);
	writeByteSR(addrHigh);	
	
}

extern void writeDataToFlash(unsigned int addr,unsigned int data){
	
	writeAddrAndData(0x5555, 0xAA);
	writeAddrAndData(0x2AAA, 0x55);
	writeAddrAndData(0x5555, 0xA0);
	writeAddrAndData(addr, data);
	
}

extern void latchAddrAndData(void){
	SRPL = 0;
	CLKSIPO = 0;
	STROBE = 1;
	STROBE = 0;
	OESIPO = 1;
	OEDATA = 1;
	
	CEFLASH = 0;
	OEFLASH = 1;
	WEFLASH = 0;
}

extern void latchAddr(void){
	SRPL = 0;
	OEDATA = 0;
	CLKSIPO = 0;
	STROBE = 1;
	STROBE = 0;
	OESIPO = 1;
	
	CEFLASH = 0;
	OEFLASH = 0;
	WEFLASH = 1;
}

extern void configFlashPorts(void){
	
}