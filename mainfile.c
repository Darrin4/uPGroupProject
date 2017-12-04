#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "delays.h"
#include "tempSensor.h"
#include <timers.h>
#include <string.h>
#include <p18f452.h>
#include "heartRate.h"
#include "Speaker.h"
#include <capture.h>
#include<math.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000UL

extern void heartYou(void);
extern void heartCal(void);
extern void resetTens(void);
extern void incTens(void);
//void modifyEnableVar(unsigned char i);
unsigned char stopped = 0;
unsigned char a = 0;
unsigned char c = 0;
unsigned char t = 0;
INT8U err;
OS_EVENT *tempSem;
//OS_EVENT *heartSem;
//OS_EVENT *binSem;
OS_STK TaskTempStk[100L];
//OS_STK TaskHeartStk[25L];

unsigned int captVal1=0,
	captVal2=0,
	riseEdge=0,
	interval=0,
	ovrflw=0,
	cnt=0,
	prev=0,
	notFirstInt=0,
	fin=0,
	nn=0,
	nn50=0;
float hrv=0;
//*****************************************************//

void DelayFor18TCY(void){
    Delay10TCYx(2);
}
void DelayPORXLCD(void){
    Delay1KTCYx(15);
}
void DelayXLCD(void){
    Delay1KTCYx(5);
}
//*****************************************************//
void options(void){
    
    while(BusyXLCD());
    SetDDRamAddr(0x10);
    putrsXLCD("                ");
    SetDDRamAddr(0x10);
    putrsXLCD("HRM - B");
    
    while(BusyXLCD());
    SetDDRamAddr(0x50);
    Delay10KTCYx(1);
    putrsXLCD("RET - A");
    
    while(BusyXLCD());
    SetDDRamAddr(0x58);
    Delay10KTCYx(1);
    putrsXLCD("RFH - 0");
}
//******************************************************//

void tempTask(void *pdata){ 
    while(stopped != 1){
        OSSemPend(tempSem, 0, &err);
        Delay10KTCYx(100);
        convertTemp();
        OSSemPost(tempSem);
    }
    OSTaskDel(OS_PRIO_SELF);
}

void initCaptureHRV(void){
    OpenTimer1 (TIMER_INT_ON & T1_16BIT_RW & T1_PS_1_1 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF & T1_SOURCE_INT & T1_SOURCE_CCP ); 
    //OpenTimer3 (TIMER_INT_OFF & T3_16BIT_RW & T3_PS_1_1 & T3_SOURCE_INT & T3_PS_1_1 & T3_SYNC_EXT_ON & T1_SOURCE_CCP); 
    OpenCapture1 (CAPTURE_INT_ON & C1_EVERY_RISE_EDGE);    
}

void displayHrv(char y[]){
    while(BusyXLCD());
    SetDDRamAddr(0x10);
    putsXLCD(y);
    Delay10KTCYx(0x26);
}

void setDispVal(void){
    int integer=0,decimal=0;
    char* y;
    integer = hrv;
    decimal = (hrv - integer)*10000;
    sprintf(y,"HRV: %d%",integer);
    displayHrv(y);
}
void configCaptPorts(void){
	PIR1bits.CCP1IF = 0;             //CCP Interrupt Flag
	PIE1bits.CCP1IE = 1;             //CCP Interrupt Enable
    IPR1bits.CCP1IP = 0;             //CCPIP Interrupt Low Priority 
    PIR1bits.TMR1IF=0;               //Timer1 Interrupt Flag initially cleared.
	IPR1bits.TMR1IP = 0;             //Timer1 Interrupt ; Low Priority 
    PIE1bits.TMR1IE = 1;             //Timer1 Interrupt Enable
    TRISCbits.TRISC2 = 1;
}

//********************************************************//

void keypadTest(void){
 if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("D");
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("#"); 
        a = 0;
        c = 0;
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("0"); 
        WriteCmdXLCD(1);
        options();
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("*");  
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("C");  
        //configCaptPorts();
        //initCaptureHRV();
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("9");  
    }else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("8");  
    }else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("7");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(10);
        if(a == 0){
            INTCONbits.INT0IE = 1;
            PIE2bits.TMR3IE = 1;
            resetTens();
            heartYou();
        }
        a = 1;
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("6");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("5");  
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("4");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        options();
    }else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("3");  
    }else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("2");  
    }else{
        while(BusyXLCD());
        SetDDRamAddr(0x40);
        Delay1KTCYx(20);
        putrsXLCD("1");  
    }
}




//*******************************************************//

void appISR(void){
    unsigned char ch;
    float intervalMs = 0;
    if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IF = 0;
        keypadTest();
        
    }
 //******************************************************//   
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        if(a == 1){
            incTens();
        }
    }
//*******************************************************//
    if(PIR2bits.TMR3IF){// check for TMR3 overflow
        PIR2bits.TMR3IF = 0;
        if(c == 19){
            c = 0;
            a = 0;
            heartCal();
            PIE2bits.TMR3IE = 0;
            INTCONbits.TMR0IE = 0;
            CloseTimer3();
        }
        c++;
    } 
//*******************************************************//
    if(1 == PIR1bits.TMR1IF){
        PIR1bits.TMR1IF=0;
        ovrflw++;
    } 
    if(1 == PIR1bits.CCP1IF){  
        PIR1bits.CCP1IF = 0;  
        if(0 == riseEdge){
            if (notFirstInt==0){
                captVal1 = ReadCapture1();
            }else{
                captVal2 = ReadCapture1();
            }
        }else if ( (1 == riseEdge) && (0 == notFirstInt)){
            captVal2 = ReadCapture1();   
        }
        riseEdge++;
        if( (riseEdge>1) || (notFirstInt==1)){
            notFirstInt=1;
            intervalMs = 65535*ovrflw + captVal2-captVal1;
            nn++;
            prev = captVal2;
            captVal1 = prev;
            intervalMs = (float)interval/(float)1000;
            if((float)intervalMs > (float)50){
                nn50++;
            }
            riseEdge = 0;
            if(nn>10){
                hrv = (float)nn50/(float)10;
                hrv = hrv*100;
                fin = 1;
                CloseCapture1();
                CloseTimer1();
                PIE1bits.CCP1IE = 0;             //CCP Interrupt Disable
                PIE1bits.TMR1IE = 0;             //Timer1 Interrupt Disable
            }   
        }
        setDispVal();
    }  
}



void main(void){
    
    OSInit();
      
    INTCONbits.GIEH = 1;
    INTCONbits.TMR0IE = 1;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    INTCONbits.INT0IF = 0;
    PIE2bits.TMR3IE = 1;
    PIR2bits.TMR3IF = 0;
    
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB0 = 1;
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC5 = 1;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC7 = 1;
    
    //configCaptPorts();
    
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
    WriteTimer0(4377);
            
    OSTaskCreate(tempTask, (void *)0, &TaskTempStk[0],1);
//  OSTaskCreate(heartTask, (void *)0, &TaskHeartStk[0],1);
    tempSem = OSSemCreate(1);
    //heartSem = OSSemCreate(1);
    //binSem = OSSemCreate(0);
    
    
    OpenXLCD(FOUR_BIT & LINES_5X7);
    while(BusyXLCD());
    WriteCmdXLCD(DON & CURSOR_OFF & BLINK_OFF); // display on
    while(BusyXLCD());
    WriteCmdXLCD(0b00000001); // display clear
    while(BusyXLCD());
    WriteCmdXLCD(ENTRY_CURSOR_INC & ENTRY_DISPLAY_NO_SHIFT);
    
    options();
        
    OSStart();
    
}
