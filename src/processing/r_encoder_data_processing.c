#include <string.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "../base/u_scheduler.h"
#include "../base/u_logger.h"
#include "../base/u_clock.h"
#include "../base/synchronization.h"
#include "../config/config.h"
#include "../../libs/ustdlib.h" //micro standard library
#include "../../libs/protoThreads/pt.h" //protothreads

#include "r_encoder_data_processing.h"
#include "../capture/r_encoder/r_encoder.h"
#include "../data/movement_db.h"
#include "../config/config.h"


int idx;
int firstIndexToProcess;
int REncoder_periodicProcessingProtoThread(struct pt* pt){
    PT_BEGIN( pt );
    
    while(1) {
        PT_WAIT_UNTIL(pt, sync_tryLock(&renc_storageLock) && (renc_samplesPendingProcessing > 0));
        firstIndexToProcess = (renc_firstFreeIndex - renc_samplesPendingProcessing)%RENC_STORAGE_SIZE;
    
        int i;
        for(i=0; i<renc_samplesPendingProcessing; i++){
            idx = (firstIndexToProcess + i) % RENC_STORAGE_SIZE;
            PT_WAIT_UNTIL(pt, mdb_tryWriteLock());
            mdb_pushPositionDifference( renc_timeSeconds[idx], renc_timeMseconds[idx], cfg_getMetersPerPulse());
            mdb_releaseWriteLock();
        }
        renc_samplesPendingProcessing = 0;
        sync_releaseLock(&renc_storageLock);
    }
    
    PT_END(pt);
}
