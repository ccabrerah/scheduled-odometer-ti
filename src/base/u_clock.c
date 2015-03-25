#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "u_scheduler.h"
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

#include "../../libs/ustdlib.h"
#include "inc/lm3s3748.h"

#include "u_logger.h"

unsigned long uClock_clockRate = 0;
unsigned long secondsSinceStart = 0;
unsigned int currentSecondTicks = 0;
tBoolean ledState = false;

void uClock_initialize(void){
    uClock_clockRate = SysCtlClockGet();
    volatile unsigned long timerPeriod = uClock_clockRate/1000;
    
    /*Configure Timer0, 1ms ticks*/ 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure( TIMER0_BASE, TIMER_CFG_32_BIT_PER);
    TimerLoadSet( TIMER0_BASE, TIMER_A, timerPeriod);
    IntEnable( INT_TIMER0A );
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
    
    
    /*led*/
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    volatile unsigned long nothing = SYSCTL_RCGC2_R;

    //
    // Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIO_PORTF_DIR_R = 0x01;
    GPIO_PORTF_DEN_R = 0x01;
    
    //
    // Turn on the LED.
    //
    GPIO_PORTF_DATA_R |= 0x01;
    ledState = true;
}

void uClock_timerIntHandler(void){
    SysTickDisable();
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  
    currentSecondTicks++;
  
    if(currentSecondTicks == 1000){
         currentSecondTicks = 0;
         secondsSinceStart++ ;
         
         //This controls the led on the evaluation, so that it changes state once every second
         if( ledState ){
         	uLogger_info("u_clock.c", "tick");
            GPIO_PORTF_DATA_R &= ~(0x01);// Turn off the LED.
            ledState = false;
         }else{
         	uLogger_info("u_clock.c", "tock");
            GPIO_PORTF_DATA_R |= 0x01;// Turn on the LED.
            ledState = true;
         }
        
    }
    
    SysTickEnable();
}

unsigned long uClock_secondsSinceStart(void){
    return secondsSinceStart;  
}

unsigned int uClock_currentMseconds(void){
    return currentSecondTicks;  
}
