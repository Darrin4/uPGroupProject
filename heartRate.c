#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "stdio.h"
#include "delays.h"
#include "timers.h"
#include "stdlib.h"
#include "heartRate.h"
#include "Speaker.h"
    
int tens;

void heartYou(void){
    TRISBbits.RB0 = 1;
    TRISBbits.RB4 = 0;
    PORTBbits.RB4 = 1;
    tens = 0;
    while(BusyXLCD());
    SetDDRamAddr(0x40);
    putrsXLCD("                ");
    Delay10KTCYx(10);
    SetDDRamAddr(0x40);
    putrsXLCD("HRM Running");
    OpenTimer3(TIMER_INT_ON & T3_16BIT_RW & T3_SOURCE_INT & T3_PS_1_8);
    WriteTimer3(3036);
    PORTBbits.RB4 = 0;
}

void heartCal(void){
    int sixtys;
    char dispC[4];
    char *bpm;
    dispC[3] = '\0';
    sixtys = 0;
    sixtys = tens*6;
    while(BusyXLCD());
    SetDDRamAddr(0x40);
    putrsXLCD("                ");
    if(sixtys<40 || sixtys>100){
        highPriorityAlarm();
    }
    TRISBbits.RB4 = 0;
    PORTBbits.RB4 = 1;
    //sprintf(bpm, "BPM:%d", sixtys);
    sprintf(bpm, "%d", sixtys);
    while(BusyXLCD());
//    WriteCmdXLCD(1);
    while(BusyXLCD());
    SetDDRamAddr(0x40);
    while(BusyXLCD());
    //Delay10KTCYx(10);
    //putsXLCD(bpm);
    putrsXLCD("BPM:");
    dispC[0] = bpm[0];
    dispC[1] = bpm[1];
    dispC[2] = bpm[2];
    putsXLCD(dispC);
    PORTBbits.RB4 = 0;   
}

void incTens(void){
    tens++;
    TRISBbits.RB4 = 0;
    PORTBbits.RB4 = 1;
    Delay10KTCYx(5);
    PORTBbits.RB4 = 0;
}

void resetTens(void){
    tens = 0;
}