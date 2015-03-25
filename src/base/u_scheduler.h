#ifndef U_SCHEDULER_H_
#define U_SCHEDULER_H_

extern void uScheduler_configure(void);

extern void uScheduler_intHandler(void);

extern void uScheduler_addProcess( void (*process)(void), int msBetweenCalls);

#endif /*U_SCHEDULER_H_*/
