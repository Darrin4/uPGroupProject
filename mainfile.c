#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "delays.h"
#include "tempSensor.h"

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL

//extern void initializeTemp();
void DelayFor18TCY(void){
    Delay10TCYx(2);
}
void DelayPORXLCD(void){
    Delay1KTCYx(15);
}
void DelayXLCD(void){
    Delay1KTCYx(5);
}

void lowIsr(void);
void highIsr(void);
void keypadTest(void);

void tempTask(void){
    
    initializeTemp();
    convertTemp();
    readScratch();
    putsXLCD(temperature);
    
}

#pragma code lowVector=0x18
void int_at_lowVector(void){
    _asm
    GOTO lowIsr
    _endasm
}
#pragma code /* return to the default code section */
#pragma interruptlow lowIsr

void lowIsr(void){

    if(INTCONbits.INT0IF){
        INTCONbits.TMR0IF = 0;
        keypadTest();
    }

}
#pragma code highVector=0x08
void highInterrupt(void){
    _asm
    GOTO highIsr
    _endasm
}
#pragma code /* return to the default code section */
#pragma interrupt highIsr
void highIsr(void){
    return;
}

void keypadTest(void){
    if(!PORTDbits.RD3 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        SetDDRamAddr(0x00);
        putrsXLCD("D");
    }else if(PORTDbits.RD3 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("#");  
    }else if(!PORTDbits.RD3 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("0");  
    }else if(PORTDbits.RD3 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("*");  
    }else if(!PORTDbits.RD3 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("C");  
    }else if(PORTDbits.RD3 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("9");  
    }else if(!PORTDbits.RD3 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("8");  
    }else if(PORTDbits.RD3 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("7");  
    }else if(!PORTDbits.RD3 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("B");  
    }else if(PORTDbits.RD3 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("6");  
    }else if(!PORTDbits.RD3 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("5");  
    }else if(PORTDbits.RD3 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("4");  
    }else if(!PORTDbits.RD3 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("A");  
    }else if(PORTDbits.RD3 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("3");  
    }else if(!PORTDbits.RD3 && PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
         SetDDRamAddr(0x00);
        putrsXLCD("2");  
    }else{
         SetDDRamAddr(0x00);
        putrsXLCD("1");  
    }
    
}

void main(void){

    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF); // display on
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
    WriteCmdXLCD(ENTRY_CURSOR_INC & ENTRY_DISPLAY_NO_SHIFT);
    
    INTCONbits.GIEH = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    TRISBbits.TRISB0 = 1;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC4 = 1;
    TRISDbits.TRISD3 = 1;
    
   
    
    
    tempTask();

}
