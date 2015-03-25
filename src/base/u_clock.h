#ifndef U_CLOCK_H_
#define U_CLOCK_H_

extern void uClock_initialize(void);

extern unsigned long uClock_secondsSinceStart(void);

extern unsigned int uClock_currentMseconds(void);

extern void uClock_timerIntHandler(void);

#endif /*U_CLOCK_H_*/
