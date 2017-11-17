#include <p18f452.h>
#include <stdio.h>
#include "delays.h"
#include <timers.h>
#include "Speaker.h"
#include <pwm.h>

extern void speakerSound(void){
	//Medical Standards : 150Hz to 3000Hz
	//For an alert frequency of 300Hz with a 4MHz Clock and prescalar of 16, the value in PR2 = 208 = 0xD0
	TRISCbits.TRISC2 = 0;
	OpenPWM1(0xD0);
	SetDCPWM1(0x68);
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);
}

extern void speakerOff(void){
	ClosePWM1();
	CloseTimer2();
}