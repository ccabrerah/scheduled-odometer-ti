#ifndef R_ENCODER_DATA_PROCESSING_H_
#define R_ENCODER_DATA_PROCESSING_H_


/**
 * Compacts/pre-processes the information captured
 * That means that the raw information that has been received since the last run is transformed into some other
 * form that makes answering queries later, less resource intensive.
 * In the future, this would be the place where the code that would archive information on permanent storage will probably be.
 * 
 * - There should be an inferior limit on the acceptable temporal proximity of two consecutive samples that steams from the maximum possible
 *  acceleration
 * - Some processes will be "listening" to a new velocity/acceleration. Probably a lock + a last updated stamp and the other process remembering
 *  when was the last time that they processed a new value   
 */
extern int REncoder_periodicProcessingProtoThread(struct pt* pt);


#endif /*R_ENCODER_DATA_PROCESSING_H_*/
