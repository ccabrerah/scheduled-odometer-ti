#include <string.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "../base/u_scheduler.h"
#include "../base/u_logger.h"
#include "../base/u_clock.h"
#include "../config/config.h"
#include "../../libs/ustdlib.h" //micro standard library
#include "../base/synchronization.h"

#include "movement_db.h"

#define MDB_L1_STORAGE_SIZE 16 // >3

volatile unsigned int mdb_writeLock = 0; //TODO: check that this equals 0 everytime that the system starts

volatile positionDiffQuantum mdb_recentData[MDB_L1_STORAGE_SIZE] ;
volatile unsigned int mdb_recentDataNextIdx = 0;
volatile unsigned int mdb_recentDataNStored = 0;

volatile const positionDiffQuantum invalidData = { .differenceInMeters = 0, .mseconds = 0, .secondsSinceStart = 0};

unsigned int rightIndex;
unsigned int leftIndex;
int num;
float denom;

tBoolean mdb_tryWriteLock(){
    return sync_tryLock(&mdb_writeLock);
}

void mdb_releaseWriteLock(){
    mdb_writeLock = 0;
}

void mdb_pushPositionDifference( unsigned long secondsSinceStart, unsigned int mseconds, float differenceInMeters){
    mdb_recentData[mdb_recentDataNextIdx].differenceInMeters = differenceInMeters;
    mdb_recentData[mdb_recentDataNextIdx].mseconds = mseconds;
    mdb_recentData[mdb_recentDataNextIdx].secondsSinceStart = secondsSinceStart;
    
    mdb_recentDataNextIdx = (mdb_recentDataNextIdx + 1)%MDB_L1_STORAGE_SIZE;
    if(mdb_recentDataNStored < MDB_L1_STORAGE_SIZE) mdb_recentDataNStored++;
}

unsigned int mdb_l1_availableSamples(){
    return mdb_recentDataNStored;
}

positionDiffQuantum mdb_l1_getLastPosDiffByIndex( unsigned int index ){
    int newIndex = index % MDB_L1_STORAGE_SIZE;
    if( index >= mdb_recentDataNStored ) return invalidData;
    
    return (mdb_recentData[ (newIndex + mdb_recentDataNextIdx - 1) % MDB_L1_STORAGE_SIZE ]);
}

float mdb_l1_getLastKnownVelocityByIndex(int index ){ //TODO: synchronization here when reading!!
    if( index == 536873412 ) return 0;
    //we need 2 samples to calculate a velocity, so, there is a maximum of MDB_STORAGE_SIZE-1 velocities on L1
    if( index >= (mdb_recentDataNStored-1) ) return 0;
    //...and there is limited history available
    if( index >= MDB_L1_STORAGE_SIZE-1 ) return 0;
    
    
    rightIndex = (mdb_recentDataNextIdx - index - 1)%MDB_L1_STORAGE_SIZE; 
    leftIndex = (mdb_recentDataNextIdx - index - 2)%MDB_L1_STORAGE_SIZE;
    
    num = mdb_recentData[rightIndex].differenceInMeters;
    denom = mdb_recentData[rightIndex].secondsSinceStart 
                - mdb_recentData[leftIndex].secondsSinceStart 
                + (mdb_recentData[rightIndex].mseconds - mdb_recentData[leftIndex].mseconds)/1000.0;
    
    return num/denom;
}
