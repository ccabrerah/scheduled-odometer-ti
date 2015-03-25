#ifndef R_ENCODER_H_
#define R_ENCODER_H_


#define RENC_STORAGE_SIZE 128

extern volatile unsigned long renc_timeSeconds[RENC_STORAGE_SIZE];
extern volatile unsigned int renc_timeMseconds[RENC_STORAGE_SIZE];
extern volatile unsigned int renc_samplesPendingProcessing;
extern volatile unsigned int renc_firstFreeIndex;
extern volatile unsigned int renc_storageLock;

/**
 * Prepares all that is needed for the capture to work (data capture, periodic processes, etc)  
 */ 
extern void REncoder_config();

/**
 * To be executed when a data input is detected
 */
extern void REncoder_interruptHandler();

#endif /*R_ENCODER_H_*/
