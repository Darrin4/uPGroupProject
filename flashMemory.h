/*Quanley Marieatte
 *file: flashMemory.h 
 *Contains the pin definitions and the function prototypes for the flash memory 
 *
 */

#ifndef FLASHMEMORY_H
#define FLASHMEMORY_H

#define SSTID			0xBF 				//SST Manufacturer's ID code
#define SSTDEVCODE		0xB7				//SST 39SF040 device code

#define OEFLASH			LATAbits.LATA5 		//Output enable pin for the Flash Memory
#define TRISOEFLASH 	TRISAbits.TRISA5     //Output enable pin for the Flash Memory
#define CEFLASH     	LATEbits.LATE0		//Chip Enable pin for flash memory
#define TRISCEFLASH		TRISEbits.TRISE0	//Chip Enable pin for flash memory
#define WEFLASH     	LATBbits.LATB2		//Write Enable pin for flash memory 
#define TRISWEFLASH		TRISBbits.TRISB2	//Write Enable pin for flash memory 

#define CLKSIPO     	LATBbits.LATB4		//Clock trigger pin for the SIPO Shift Registers
#define TRISCLKSIPO		TRISBbits.TRISB4    //Clock trigger pin for the SIPO Shift Registers
#define TRISSTROBE		TRISAbits.TRISA2 	//Strobe pin for SIPO shift registers. On rising edge, 
#define STROBE			LATAbits.LATA2 		//Strobe pin for SIPO shift registers. On rising edge, 
												//	data is latched from the register stages
#define OESIPO      	LATBbits.LATB3		//Output enable pin for the SIPO Shift Registers
#define TRISOESIPO  	TRISBbits.TRISB3    //Output enable pin for the SIPO Shift Registers

#define OEDATA      	LATEbits.LATE1		//Output enable pin for the data Shift Register
#define TRISOEDATA  	TRISEbits.TRISE1    //Output enable pin for the data Shift Register
#define DATASIPO    	LATAbits.LATA3		//Pin for the data to the SIPO shift registers
#define TRISDATASIPO 	TRISAbits.TRISA3	//Pin for the data to the SIPO shift registers

#define SRPL        	LATDbits.LATD3		//Parallel load enable pin for PISO shift register
#define TRISSRPL    	TRISDbits.TRISD3    //Parallel load enable pin for PISO shift register
#define CP1 			LATCbits.LATC0 		//Clock 1 pin for PISO SR
#define TRISCP1 		TRISCbits.TRISC0	//Clock 1 pin for PISO SR
#define CP2 			LATEbits.LATE2 		//Clock 2 pin for PISO SR
#define TRISCP2 		TRISEbits.TRISE2	//Clock 2 pin for PISO SR
#define DATAPISO 		LATAbits.LATA6		//Pin for the data from the PISO shift register
#define TRISDATAPISO 	TRISAbits.TRISA6	//Pin for the data from the PISO shift register

extern unsigned char readBytePISO();
extern unsigned char readFlash(unsigned int addr);

extern void writeByteSR(unsigned int data);
extern void writeAddrSIPO(unsigned int addr);
extern void writeAddrAndData(unsigned int addr, unsigned char data);
extern void writeDataToFlash(unsigned int addr,unsigned char data);//, unsigned int sector);

extern void latchAddr(void);
extern void latchAddrAndData(void);
extern void latchDataOnFlash(void);

extern void setCheckFlashMode(void);
extern void setSectorEraseMode(void);
extern void setReadFlashMode(void);
extern void revertCheckFlashMode(void);

extern unsigned int checkSectorErase(void);
extern unsigned int checkSectorWrite(unsigned char data);

extern void enableDataSROutput(void);
extern void disableDataSROutput(void);

extern void eraseSector(unsigned int sector);

extern char checkFlashMemoryPresent(void);

extern void sectorWriteByte(unsigned int sector,unsigned int address, unsigned int data);
extern char sectorReadByte(unsigned int address);

extern void configFlashPorts(void);

#endif