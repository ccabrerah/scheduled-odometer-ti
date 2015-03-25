#include "synchronization.h"

unsigned int __LDREXW(volatile unsigned int *addr){
    __asm(" ldrex   r0, [r0]\n"
    "   bx  lr");
    return (0);
}

unsigned int __STREXW(unsigned int value, volatile unsigned int *addr){
    __asm("   strex r0, r0, [r1]\n"
    "   bx lr");
    return (0);
}

tBoolean sync_tryLock(volatile unsigned int *lockAddr){
    unsigned int oldval;

    oldval = __LDREXW(lockAddr);
    if( (oldval == LOCK_TAKEN) || __STREXW(LOCK_TAKEN, lockAddr) != 0 ){
        return false;
    }else{
        return true;
    }
}

void sync_releaseLock(volatile unsigned int *lockAddr){
    *lockAddr = LOCK_FREE;
}
