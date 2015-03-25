#ifndef DATA_QUERY_H_
#define DATA_QUERY_H_

/**
 * Estimated velocity now. 
 * - There is an upper limit that depends on the amount of time that has passed since the last Rotary Encoder pulse
 * - Extrapolate using last known acceleration?
 */
extern float getVelocity();

/**
 * Last known velocity 
 * (Spurious data should not exist at this point)
 */
extern float getLastVelocity();

/**
 * (lineally?) Interpolated velocity at the requested time
 */
extern ? getVelocityAt(unsigned long seconds, unsigned int mSeconds);

/**
 * Estimated acceleration now. 
 * - There is an upper limit that depends on the amount of time that has passed since the last Rotary Encoder pulse
 */
extern float getVelocity();

/**
 * Last known acceleration 
 * (Spurious data should not exist at this point)
 */
extern float getLastAcceleration();

/**
 * (lineally?) Interpolated acceleration at the requested time
 */
extern float getAccelerationAt(unsigned long seconds, unsigned int mSeconds);


#endif /*DATA_QUERY_H_*/
