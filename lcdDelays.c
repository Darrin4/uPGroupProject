#include <delays.h>
#include "xlcd.h"

extern void DelayFor18TCY(void){
    Delay10TCYx(2);
}
extern void DelayPORXLCD(void){
    Delay1KTCYx(15);
}
extern void DelayXLCD(void){
    Delay1KTCYx(5);
}
