#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "stdio.h"
#include "delays.h"
#include "ow.h"


unsigned char tempMSB = 0;
unsigned char tempLSB = 0;
unsigned char temp = 0;
float fraction = 0.0;
int  fractionInt = 0;
int sign = 0;
char temperature[20];
unsigned char deg = 0xDF;
void initializeTemp(void);

void initializeTemp(void){
    
    TRISCbits.TRISC2 = 0;
    PORTCbits.RC2 = 0;      //Port for pull-up   
    ow_reset();            //Master issues Reset Pulse
}

void convertTemp(void){
    ow_write_byte(0xCC);    //Master issues SKIP ROM command
    ow_write_byte(0x44);    //Master issues Convert T command
    PORTEbits.RE0 = 1;      //Enable pull-up
    Delay1KTCYx(800);
    PORTCbits.RC2 = 0;      //Disable strong pull-up
}

void readScratch(void){
    ow_write_byte(0xCC);
    ow_reset();
    ow_write_byte(0xBE);
    tempLSB = ow_read_byte(); //Master reads byte 0 of scratchpad
    tempMSB = ow_read_byte(); //Master reads byte 1 of scratchpad
    ow_reset();
    
    tempLSB >> 4;
    tempMSB << 4;
    temp = tempLSB | tempMSB;
    
    if(tempLSB & 0x01){fraction += 0.0625;}
    if(tempLSB & 0x02){fraction += 0.125;}
    if(tempLSB & 0x04){fraction += 0.25;}
    if(tempLSB & 0x08){fraction += 0.5;}
    fractionInt = fraction*1000;

    sign = ((tempMSB >> 3 )& 0x3F);
    if(sign == 0){
        sprintf(temperature,"+%d.%d%cC",temp,fractionInt,deg);
        temp = 0;
        fractionInt= 0;
        fraction =0.0;
        
    }
    else{
        sprintf(temperature,"-%d.%d%cC",temp,fractionInt);
        temp = 0;
        fractionInt= 0;
        fraction =0.0;

    }
    
    
}


