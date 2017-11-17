#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "stdio.h"
#include "delays.h"
#include "timers.h"
#include "stdlib.h"

void heartYou(void){
    TRISBbits.RB0 = 1;
    
    int sixtys;
    int tens = 0;
    extern unsigned char a;
    int i = 0;
    Delay1KTCYx(20);
    while(BusyXLCD());
    SetDDRamAddr(0x40);
    putrsXLCD("Heart Rdr Ready");
    poll:
    if(!a){
        goto poll;
    }else{
        for(i=0; i<3; i++){
            OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
            WriteTimer0(-9934465);
            while(!INTCONbits.TMR0IF){
                if(PORTBbits.RB0){
                    tens++;
                }
            }   
            tens = tens*6;
            sixtys += tens;
        }
        sixtys = sixtys/3;
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        putrsXLCD("               ");
        Delay10KTCYx(1);
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        putrsXLCD("BPM - &d",sixtys);
        Delay10KTCYx(1);
        
        
    }
}
