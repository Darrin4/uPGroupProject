#include <p18f452.h>
#include <stdio.h>
#include <timers.h>
#include <pwm.h>
#include <delays.h>
#include "Speaker.h"


extern void speakerSound(int freq){
	//Medical Standards : 150Hz to 3000Hz
	//For an alert frequency of 300Hz with a 4MHz Clock and prescalar of 16, the value in PR2 = 208 = 0xD0
    int period = (int)((1.0/(float)freq)/(16.0*0.000001));
	TRISCbits.TRISC1 = 0;
	OpenPWM2(period);//0xD0);
	SetDCPWM2(period/2);//x68);
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);
}

extern void speakerOff(void){
	TRISCbits.TRISC1 = 1;
    ClosePWM1();
	CloseTimer2();
}