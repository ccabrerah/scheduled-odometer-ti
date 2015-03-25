#ifndef MOVEMENT_DB_H_
#define MOVEMENT_DB_H_
#include "inc/hw_types.h"
/**
 * This saves the information about the car position´s evolution. 
 * Only some of this information is stored on-memory, and layed-out for fast use("L1");
 * more extensive position information could be stored on external memory("L2", possibly 
 * in compressed form), for example, saved on the on-chip flash or in a usb connected 
 * mass-storage device.
 */

typedef struct{
    unsigned long secondsSinceStart;
    unsigned int mseconds;
    float differenceInMeters;
} positionDiffQuantum;

/**
 * -----------SYNCHRONIZATION-----------
 */
extern tBoolean mdb_tryWriteLock(); 

extern void mdb_releaseWriteLock(); 

/**
 * -------------WRITING-----------------
 */
 
/**
 * This function should be called only when holding the write lock
 */
extern void mdb_pushPositionDifference( unsigned long secondsSinceStart, unsigned int mseconds, float differenceInMeters);

/**
 * ----------READING(Misc)-------------
 */
extern unsigned int mdb_l1_availableSamples(); 

/**
 * ---------READING(Position)----------
 */
extern positionDiffQuantum mdb_l1_getLastPosDiffByIndex( unsigned int index );//0 is the most recent, 1 the next one, and so on...

//extern positionDiffQuantum mdb_l2_getNearestPosDiffByIndex( unsigned int index );

//extern positionDiffQuantum mdb_l2_getPosDiffByIndexRelativeTo( positionDiffQuantum refData, unsigned int index );

/**
 * ---------READING(Velocity)----------
 */
 extern float mdb_l1_getLastKnownVelocityByIndex(int index);

#endif /*MOVEMENT_DB_H_*/
