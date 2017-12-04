#include <p18f452.h>
#include <float.h>
#include <stdio.h>
#include <delays.h>
#include "ow.h"


void convertTemp(void){
    unsigned char tempMSB = 0;
    unsigned char tempLSB = 0;
    unsigned char tempLow = 0;
    unsigned char tempHigh = 0;
    unsigned char temp = 0;
    int fraction = 0;
    int sign = 0;
    char temperature[20];
    unsigned char deg = 0xDF;
    
    TRISCbits.TRISC3 = 0;
    PORTCbits.RC3 = 0;      //Port for pull-up   
    ow_reset();            //Master issues Reset Pulse

    ow_write_byte(0xCC);    //Master issues SKIP ROM command
    ow_write_byte(0x44);    //Master issues Convert T command
    PORTCbits.RC3 = 1;      //Enable pull-up
    Delay10KTCYx(100);
    PORTCbits.RC3 = 0;      //Disable strong pull-up
    ow_reset();
    ow_write_byte(0xCC);
    ow_write_byte(0xBE);
    tempLSB = ow_read_byte();   //Master reads byte 0 of scratchpad
    tempMSB = ow_read_byte();   //Master reads byte 1 of scratchpad
    
    tempLow = tempLSB >> 4;
    tempHigh = tempMSB << 4;
    temp = tempHigh|tempLow;
    
    if(tempLSB & 0x01)          //Check bit 2^-4 
        fraction += 62;     //Add if bit =1
    if(tempLSB & 0x02)          //Check bit 2^-3
        fraction += 125;      //Add if bit =1
    if(tempLSB & 0x04)          //Check bit 2^-2
        fraction += 250;       //Add if bit =1
    if(tempLSB & 0x08)          //Check bit 2^-1
        fraction += 500;        //Add if bit =1
    sign = (tempMSB & 0xF8);    //check if sign bits are enabled
    if(sign == 0){
        sprintf(temperature, "Temp +%d.%d%cC", temp, fraction, deg);
     
    }else{
        sprintf(temperature, "Temp -%d.%d%cC", temp, fraction, deg);
    }
    temp = 0;
    fraction =0;
    while(BusyXLCD());
    SetDDRamAddr(0x00);
    Delay10KTCYx(10);
    putsXLCD(temperature); 
}

