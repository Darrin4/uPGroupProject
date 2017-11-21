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
    float fraction = 0.0;
    int  fractionInt = 0;
    int sign = 0;
    char temperature[20];
    unsigned char deg = 0xDF;
    
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;      //Port for pull-up   
    ow_reset();            //Master issues Reset Pulse

    ow_write_byte(0xCC);    //Master issues SKIP ROM command
    ow_write_byte(0x44);    //Master issues Convert T command
    PORTCbits.RC2 = 1;      //Enable pull-up
    Delay10KTCYx(100);
    PORTCbits.RC2 = 0;      //Disable strong pull-up
    ow_reset();
    ow_write_byte(0xCC);
    ow_write_byte(0xBE);
    tempLSB = ow_read_byte(); //Master reads byte 0 of scratchpad
    tempMSB = ow_read_byte(); //Master reads byte 1 of scratchpad
    
    tempLow = tempLSB >> 4;
    tempHigh = tempMSB << 4;
    temp = tempLow|tempHigh;
    
    if(tempLSB & 0x01)
        fraction += 0.0625;
    if(tempLSB & 0x02)
        fraction += 0.125;
    if(tempLSB & 0x04)
        fraction += 0.25;
    if(tempLSB & 0x08)
        fraction += 0.5;
    fractionInt = fraction*1000;
    sign = ((tempMSB >> 3 )& 0x3F);
    if(sign == 0){
        sprintf(temperature,"+%d.%d%cC",temp,fractionInt,deg);
        temp = 0;
        fractionInt= 0;
        fraction =0.0;     
    }else{
        sprintf(temperature,"-%d.%d%cC",temp,fractionInt);
        temp = 0;
        fractionInt= 0;
        fraction =0.0;
    }
    while(BusyXLCD());
    WriteCmdXLCD(1);
    while(BusyXLCD());
    SetDDRamAddr(0x00);
    putsXLCD(temperature);  
    //OSTimeDlyHMSM ( 0, 0, 1, 0);
    
}

