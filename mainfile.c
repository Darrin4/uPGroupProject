#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "delays.h"
#include "tempSensor.h"
#include <timers.h>
#include <xlcd.h>
#include <string.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL

unsigned char stopped = 0;

//INT8U err;
//OS_EVENT *tempSem;
//OS_EVENT *tempEgSem;
OS_STK TaskTempStk[100L];
//OS_STK TaskTemp2Stk[100L];

void DelayFor18TCY(void){
    Delay10TCYx(2);
}
void DelayPORXLCD(void){
    Delay1KTCYx(15);
}
void DelayXLCD(void){
    Delay1KTCYx(5);
}

void tempTask(void *pdata){
    while(stopped != 1){
        while(BusyXLCD());
        WriteCmdXLCD(0b00000001);
        convertTemp();
        //OSSemPend(tempSem, 0, &err);
        OSTimeDlyHMSM ( 0, 0, 1, 0);
       // OSSemPost(tempSem);
        OSTimeDlyHMSM ( 0, 0, 100, 0);
    }
    OSTaskDel(OS_PRIO_SELF);
}
/*
void tempEg(void *pdata){
    while(stopped != 1){
        OSSemPend(tempEgSem, 0, &err);
        while(BusyXLCD());
        WriteCmdXLCD(0b00000001);
        SetDDRamAddr(0x40);
        OSTimeDlyHMSM ( 0, 0, 1, 0);
        putrsXLCD("eg");
        OSTimeDlyHMSM ( 0, 0, 100, 0);
        OSSemPost(tempSem);
        OSTimeDlyHMSM ( 0, 0, 1, 0);
    }
    OSTaskDel(OS_PRIO_SELF);
}
*/
void keypadTest(void){
    if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("D");
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("#");  
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("0");  
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("*");  
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("C");  
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("9");  
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("8");  
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("7");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("B");  
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("6");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("5");  
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("4");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("A");  
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("3");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("2");  
    }else{
        while(BusyXLCD());
        SetDDRamAddr(0x00);
        Delay1KTCYx(20);
        putrsXLCD("1");  
    }
    
}

void appISR(void){
    if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        keypadTest();
    }
}

void main(void){
    
    OSInit();
            
    OSTaskCreate(tempTask, (void *)0, &TaskTempStk[0], 1);
  //  OSTaskCreate(tempEg, (void *)0, &TaskTemp2Stk[0], 2);
    //tempSem = OSSemCreate(0);
  
    
   // OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
   // WriteTimer0(4377);
    
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF); // display on
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
    WriteCmdXLCD(ENTRY_CURSOR_INC & ENTRY_DISPLAY_NO_SHIFT);
    

       
    INTCONbits.GIEH = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB0 = 1;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC7 = 1;
    
    OSStart();
}
