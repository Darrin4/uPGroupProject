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
    ClosePWM2();
	CloseTimer2();
}

extern void beepSpeaker(int note, int delay){
	//This sound produces a beep on and off for the time delay in ms. 
	speakerSound(note);
	Delay1TCYx(delay);
	speakerOff();
	Delay1TCYx(delay);
}

extern void highPriorityAlarm(void){
	beepSpeaker(C4, 125);
	beepSpeaker(C4, 125);
	beepSpeaker(C4, 125);
	Delay1TCYx(375);	  	//This produces and off time of 500ms the beepSpeaker(C4, 125) puts the speaker off for 125ms so 375ms are left for the 500ms off-time.
	
	beepSpeaker(C4, 125);
	beepSpeaker(C4, 125);
	Delay10TCYx(87);		//This produces and off time of 1000ms the beepSpeaker(C4, 125) puts the speaker off for 125ms so 875ms are left for the 1000ms off-time.
}

extern void mediumPriorityAlarm(void){
	beepSpeaker(C4, 125);
	beepSpeaker(C4, 125);
	beepSpeaker(C4, 125);
	Delay1TCYx(375);
}

extern void lowPriorityAlarm(void){
	beepSpeaker(E4, 250);
	beepSpeaker(C4, 125);
}

