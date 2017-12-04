#ifndef SPEAKER_COMP_H
#define SPEAKER_COMP_H

#define C4 	262
#define D4	294
#define E4	330
#define F4	349
#define Fs4	370
#define G4	392
#define A4	440
#define B4	494
#define C5	523

extern void speakerSound(int freq);
extern void speakerOff(void);

extern void beepSpeaker(int note, int delay);

extern void highPriorityAlarm(void);
extern void mediumPriorityAlarm(void);
extern void lowPriorityAlarm(void);


#endif
