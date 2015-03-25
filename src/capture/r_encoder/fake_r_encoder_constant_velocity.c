#include <string.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "../../base/u_scheduler.h"
#include "../../base/u_logger.h"
#include "../../base/u_clock.h"
#include "../../base/synchronization.h"
#include "../../config/config.h"
#include "../../../libs/ustdlib.h" //micro standard library

#include "r_encoder.h"

#define FRE_TMP_STORAGE_SIZE 24

volatile unsigned long renc_timeSeconds[RENC_STORAGE_SIZE];
volatile unsigned int renc_timeMseconds[RENC_STORAGE_SIZE];
volatile unsigned int renc_samplesPendingProcessing;
volatile unsigned int renc_firstFreeIndex;
volatile unsigned int renc_storageLock;

unsigned int fre_usedTmpStorage = 0;
unsigned long fre_tmpSeconds[FRE_TMP_STORAGE_SIZE];
unsigned int fre_tmpMseconds[FRE_TMP_STORAGE_SIZE];

/*misc variables*/
char fre_textToDisplay[71];
unsigned long nowSecs;
unsigned int nowMsecs;
int i;
float fakeVelocity;
unsigned int metersPerPulse;
float pulsesPerSecond;
float pulsePeriodMsecs;

/**
 * (not in interface)Inserts one pulse
 */
void REncoder_fakeDataGenerator(void){
    //get current time
    nowSecs = uClock_secondsSinceStart();
    nowMsecs = uClock_currentMseconds();

    //write information to tmp area
    renc_timeSeconds[fre_usedTmpStorage] = nowSecs;
    renc_timeMseconds[fre_usedTmpStorage] = nowMsecs;
    fre_usedTmpStorage++;
    
    //try to get lock and copy data to shared area
    if(sync_tryLock(&renc_storageLock)){
        for(i=0; i<fre_usedTmpStorage; i++){
            renc_timeSeconds[renc_firstFreeIndex] = fre_tmpSeconds[i];
            renc_timeMseconds[renc_firstFreeIndex] = fre_tmpMseconds[i];
            renc_samplesPendingProcessing++;
            renc_firstFreeIndex++;
            if(renc_firstFreeIndex > RENC_STORAGE_SIZE) renc_firstFreeIndex = 0;
            if(renc_samplesPendingProcessing > RENC_STORAGE_SIZE) renc_firstFreeIndex = 0;
        }
        fre_usedTmpStorage=0;
        sync_releaseLock(&renc_storageLock);
    }
    
}

/**
 * Prepares all that is needed for the capture to work (data capture, periodic processes, etc)  
 */ 
void REncoder_config(){
    renc_samplesPendingProcessing = 0;
    renc_firstFreeIndex = 0;
    renc_storageLock = false;
    
    //constant speed
    fakeVelocity = 13.8888889;//m/s (~50 km/h)
    
    //calculate period that produces the desired speed
    metersPerPulse = cfg_getMetersPerPulse();
    pulsesPerSecond = fakeVelocity * metersPerPulse;
    pulsePeriodMsecs = 1000/pulsesPerSecond;

    usnprintf( fre_textToDisplay, 70, "Calculated period for inserting fake pulses: %4dms", pulsePeriodMsecs);
    uLogger_info("fake_r_encoder_constant_velocity.c", fre_textToDisplay);
    
    uScheduler_addProcess( &REncoder_fakeDataGenerator, pulsePeriodMsecs );
}

/**
 * To be executed when a data input is detected
 */
void REncoder_interruptHandler(){
    //do nothing, not used
}
