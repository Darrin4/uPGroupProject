#include <p18f452.h>
#include <float.h>
#include <stdio.h>
#include <delays.h>
#include "flashMemory.h"

extern unsigned char readBytePISO(){
	unsigned char data = 0;
	int shifts = 0;
						//Start the parallel load sequence
	SRPL 	= 0;
	CP2 	= 1;		//CP2 is being used as a clock inhibit. 
						//Disable the data output SIPO SR to prevent bus contention
	disableDataSROutput();
						//Enable the write sequence on the flash RAM
    WEFLASH = 1;
	CEFLASH = 0;
	OEFLASH = 0;
	
	Delay10TCYx(15);	//Give the signals time to settle.
    
	CP2 = 0;			//Disable the clock inhibit
	
	SRPL = 1;
	CP1 = 0;
	
	for (shifts = 0; shifts < 8; shifts ++){
		data |= DATAPISO;
		data = data << 1;
						//pulse the clock to shift the data
		CP1 = 1;
        CP1 = 0;
	}
						//Disable the read on the flash memory
	OEFLASH = 1;
	CEFLASH = 1;
	return (data);
}

extern unsigned char readFlash(unsigned int addr){
	unsigned char data;
	setReadFlashMode(); 
	writeAddrSIPO(addr);
	data = readBytePISO();
	
	return (data);
}

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

extern void writeAddrSIPO(unsigned int addr){
	unsigned int addrHigh = (addr >> 16);
	unsigned int addrMed = (addr >> 8);
	unsigned int addrLow = (addr); 
	
	OEDATA = 0;
	OESIPO = 0;
	
	writeByteSR(addrLow);
	writeByteSR(addrMed);
	writeByteSR(addrHigh);	
	latchAddr();
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
	latchDataOnFlash();
}

extern void writeDataToFlash(unsigned int addr,unsigned char data){//, unsigned int sector){
	eraseSector(addr);
	writeAddrAndData(0x5555, 0xAA);
	writeAddrAndData(0x2AAA, 0x55);
	writeAddrAndData(0x5555, 0xA0);
	writeAddrAndData(addr, data);
	//while (0 == checkSectorWrite(data));	
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

extern void latchDataOnFlash(void){
	//Latch address
    WEFLASH = 0;
    CEFLASH = 0;
    
    //Latch data
    WEFLASH = 1;
    CEFLASH = 1;
    

    //Write Commmand
    WEFLASH = 1;
    CEFLASH = 0;
    
    //Restore Pins
    WEFLASH = 1;
    CEFLASH = 1;
}

extern void setCheckFlashMode(void){
    disableDataSROutput();
    OEFLASH=1;
    WEFLASH = 1;
    CEFLASH =1;
    return;
}

extern void setSectorEraseMode(void){
    disableDataSROutput();
    OEFLASH=1;
    WEFLASH = 0;
    CEFLASH =0;
    return;
}

extern void setReadFlashMode(void){
    enableDataSROutput();
    WEFLASH=1;
    CEFLASH=1;
    OEFLASH=1;
    Delay1TCY();
    CEFLASH=0;
    OEFLASH=0;
    return;
}

extern void revertCheckFlashMode(void){
    enableDataSROutput();
    WEFLASH=0;
    CEFLASH=1;
    OEFLASH=1;
    return;
}

extern unsigned int checkSectorErase(void){
/* 	checkSectorErase returns 0 if the sector is not yet erased 
 *	and 1 if the sector is erased.
 */
	unsigned int flashDQ7 = 0;
	unsigned int status = 0;

	disableDataSROutput();
	Delay1KTCYx(25);   //delay Tsce for sector chip erase 

	CEFLASH = !CEFLASH; //Toggling of these lines are necessary for polling
    OEFLASH = !OEFLASH; //Consult Figure 8 in spec sheet 
    WEFLASH = 1;

    flashDQ7 = readBytePISO();

    if(0xFF==flashDQ7){
    	status = 1;
    }
   	else{
        status = 0;
    }

	return (status);
}

extern unsigned int checkSectorWrite(unsigned char data){
/* 	checkSectorWrite returns 0 if the sector is not written 
 *	and 1 if the sector is written correctly.
 */
	unsigned int flashDQ7 = 0;
	unsigned int flashDQ7Comp = 0;
	unsigned int status = 0;

	disableDataSROutput();
	Delay10TCYx(2);   //delay Tbp for byte program 

	CEFLASH = !CEFLASH; //Toggling of these lines are necessary for polling
    OEFLASH = !OEFLASH; //Consult Figure 8 in spec sheet 
    WEFLASH = 1;

    flashDQ7 = readBytePISO();
    flashDQ7Comp = !data;

    if(0 == ((flashDQ7 >> 7)^(flashDQ7Comp >> 7))){
        status = 0;
    }
    else{
        status = 1;
    }

	return (status);
}

extern void enableDataSROutput(void){
	OEDATA = 1;
}

extern void disableDataSROutput(void){
	OEDATA = 0;
}

extern void eraseSector(unsigned int sector){
    setSectorEraseMode();
	sector = sector << 12;
    sector = sector & 0x7F000;
    writeAddrAndData(0x5555, 0xAA);
    writeAddrAndData(0x2AAA, 0x55);
    writeAddrAndData(0x5555, 0x80);
    writeAddrAndData(0x5555, 0xAA);
    writeAddrAndData(0x2AAA, 0x55);
    writeAddrAndData(sector, 0x30);
    //while (0 == checkSectorErase());
}


extern char checkFlashMemoryPresent(void){
    
    unsigned int manufacturerID =0;
    unsigned int deviceID =0;
    unsigned char SSTPresent=0;
    setCheckFlashMode();
    
    //Write data block first, 1st Bus Write Cycle
    //Software ID Entry
    writeAddrAndData(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    writeAddrAndData(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    writeAddrAndData(0x5555,0x90);
    disableDataSROutput();

    //Write address lines to read manufacturer ID
    writeAddrAndData(0x0000,0x00);
    manufacturerID = readBytePISO();

    //Write address lines to read device ID
    writeAddrAndData(0x0001,0x00);
    deviceID = readBytePISO();
    
    if ((manufacturerID == SSTID) && (deviceID ==SSTDEVCODE)){
        SSTPresent=0xFF;
    }
    else{
        SSTPresent=0xAA;
    }
        
    //Software ID Exit
    enableDataSROutput();  //Only need to call once to turn on the data shift reg
                       //Since no reads are being done after this operation
    //Write data block first, 1st Bus Write Cycle
    writeAddrAndData(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    writeAddrAndData(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    writeAddrAndData(0x5555,0xF0);
    
    Delay1KTCYx(100);
    return (SSTPresent);
}

 //TO EDIT STILL
extern void sectorWriteByte(unsigned int sector,unsigned int address, unsigned int data){
    //if(SSTPresent==0xFF){
    eraseSector(sector);
    setCheckFlashMode();
    //Write data block first, 1st Bus Write Cycle
    writeAddrAndData(0x5555,0xAA);
    //Write data block first, 2nd Bus Write Cycle
    writeAddrAndData(0x2AAA,0x55);
    //Write data block first, 3rd Bus Write Cycle
    writeAddrAndData(0x5555,0xA0);
    //Write data block first, 4th Bus Write Cycle
    writeAddrAndData(address,data);
    //while (0 == checkSectorWrite(data));
    //checkSectorWrite(data);
    revertCheckFlashMode();
    //return 1;
    //}
    //else{
     //   return 0;
    //}
}

extern char sectorReadByte(unsigned int address){//, char SSTPresent){
    unsigned int readByte=0;
    setReadFlashMode();
    //if(SSTPresent==0xFF){
    writeAddrAndData(address,0x00);
    readByte =readBytePISO();
    return (readByte);
}
    //return (1);
    //}
    //else{
    // 	return (0);   
    //}

extern void configFlashPorts(void){
	//Flash Enable Pins
	TRISCEFLASH = 0;
    TRISWEFLASH	= 0;
	TRISOEFLASH = 0;
	//SIPO Shift Register Control Pins (Output Data)
    TRISCLKSIPO = 0;
    TRISSTROBE = 0;
    TRISOESIPO  = 0;
    TRISOEDATA  = 0;
    TRISDATASIPO = 0; //Data output
    //PISO Shift Register Control Pins (Input Data)
    TRISSRPL = 0;
    TRISCP1 = 0;
    TRISCP2 = 0;
    TRISDATAPISO = 1; //Data input
}