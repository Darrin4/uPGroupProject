#ifndef SPEAKER_COMP_H
#define SPEAKER_COMP_H

#include <p18f452.h>
#include <stdio.h>
#include "delays.h"
#include <timers.h>
#include "Speaker.h"
#include <pwm.h>

extern void speakerSound(void);
extern void speakerOff(void);

#endif
