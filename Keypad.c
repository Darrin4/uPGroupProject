#include "app_cfg.h"
#include "INCLUDES.H"
#include "float.h"
#include "delays.h"
#include "tempSensor.h"
#include <timers.h>
#include <string.h>
#include "keypad.h"

extern char keypadChar(void){
    if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        return('D');
    }
	else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        return('#');  
    }
	else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        return('0');  
    }
	else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        return('*');  
    }
	else if(!PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        return('C');  
    }
	else if(!PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        return('9');  
    }
	else if(!PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        return('8');  
    }
	else if(!PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        return('7');  
    }
	else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        return('B');  
    }
	else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && !PORTCbits.RC6){
        return('6');  
    }
	else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        return('5');  
    }
	else if(PORTCbits.RC7 && PORTCbits.RC4 && PORTCbits.RC5 && !PORTCbits.RC6){
        return('4');  
    }
	else if(PORTCbits.RC7 && !PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        return('A');  
    }
	else if(PORTCbits.RC7 && PORTCbits.RC4 && !PORTCbits.RC5 && PORTCbits.RC6){
        return('3');  
    }
	else if(PORTCbits.RC7 && !PORTCbits.RC4 && PORTCbits.RC5 && PORTCbits.RC6){
        return('2');  
    }
	else{
        return('1');  
    }
}
