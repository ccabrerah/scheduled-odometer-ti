#ifndef SYNCHRONIZATION_H_
#define SYNCHRONIZATION_H_
#include "inc/hw_types.h"
#define LOCK_TAKEN 1
#define LOCK_FREE 0

extern unsigned int  __LDREXW(volatile unsigned int *addr);

extern unsigned int  __STREXW(unsigned int value, volatile unsigned int *addr);

extern tBoolean sync_tryLock(volatile unsigned int *lockAddr);

extern void sync_releaseLock(volatile unsigned int *lockAddr);
  
#endif /*SYNCHRONIZATION_H_*/
