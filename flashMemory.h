/*Quanley Marieatte
 *file: flashMemory.h 
 *Contains the pin definitions and the function prototypes for the flash memory 
 *
 */

#ifndef FLASHMEMORY_H
#define FLASHMEMORY_H

#define CEFLASH		//Chip Enable pin for flash memory
#define WEFLASH		//Write Enable pin for flash memory 
#define CLKSIPO		//Clock trigger pin for the SIPO Shift Registers
#define STROBE		//Strobe pin for SIPO shift registers. On rising edge, 
					//	data is latched from the register stages
#define OESIPO		//Output enable pin for the SIPO Shift Registers
#define OEDATA 		//Output enable pin for the data Shift Register
#define OEFLASH		//Output enable pin for the Flash Memory
#define DATASIPO	//Pin for the data to the SIPO shift registers
#define SRPL		//Parallel load enable pin for PISO shift register
#define CP1 		//Clock 1 pin for PISO SR
#define CP2 		//Clock 2 pin for PISO SR
#define DATAPISO 	//Pin for the data from the PISO shift register

extern void writeByteSR(unsigned int data);
extern unsigned char readBytePISO();
extern unsigned char readFlash(unsigned int addr);
extern void writeAddrAndData(unsigned int addr, unsigned char data);
extern void writeAddrSIPO(unsigned int addr);
extern void writeDataToFlash(unsigned int addr,unsigned int data);
extern void latchAddrAndData(void);
extern void latchAddr(void);
extern void configFlashPorts(void);

#endif