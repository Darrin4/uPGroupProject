//Quanley Marieatte - MicroP Lab 1
#include <p18f452.h>
#include <stdio.h>
#include <delays.h>
#include <timers.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "xlcd.h"
//#include "tempSensor.h"
#include "Speaker.h"
#include "Keypad.h"

#define _XTAL_FREQ 4000000

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF


void appISRHigh(void);
void appISRLow(void);

#pragma code high_vector=0x08
void interrupt_at_high_vector(void){
    _asm GOTO appISRHigh _endasm
} 
#pragma code
#pragma interrupt appISRHigh
void appISRHigh(void){
    char* msg;    
    if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        sprintf(msg,"Char Pressed:%c",keypadChar());
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        while(BusyXLCD());
        putsXLCD(msg);
        speakerSound(3000);
        Delay1KTCYx(25);
        speakerOff();
    }
}

#pragma code low_vector=0x18
void interrupt_at_low_vector(void){
    _asm GOTO appISRLow _endasm
}
#pragma code
#pragma interruptlow appISRLow
void appISRLow(void){
   
}

void initInterrupts(void);
void initPorts(void);
void configLCD(void);
void initPBInterrupts(void);

static unsigned char a;

void main (void){
    initPorts();
    initInterrupts();
    configLCD();
    
    while(1){
        LATBbits.LATB4 = !LATBbits.LATB4;
        Delay10KTCYx(100);
        LATBbits.LATB4 = !LATBbits.LATB4;
        Delay10KTCYx(100);
    }
}

void initInterrupts(void){
    INTCONbits.GIE = 0;
    RCONbits.IPEN = 0;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    //INTCONbits.TMR0IE = 0;
    initPBInterrupts();
    INTCONbits.GIEH = 1;
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
}
void initPorts(void){
    //PortA Pins
    //PortB Pins
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB3 = 1;    
    TRISBbits.TRISB4 = 0;
    //PortC Pins
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC7 = 1;
    //PortD Pins
    //PortE Pins
    
}
void initPBInterrupts(void){
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IP = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT2IF = 0;
    INTCON3bits.INT2IP = 0;
    INTCON3bits.INT2IE = 0;
}
void configLCD(void){
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF); // display on
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
    //WriteCmdXLCD(ENTRY_CURSOR_INC & ENTRY_DISPLAY_NO_SHIFT);
}


