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


#include "u_logger.h"

#define CYCLES_BETWEEN_RUNS 1000000
#define MAXIMUM_NUMBER_OF_PROCESSES 10


void (*processes[MAXIMUM_NUMBER_OF_PROCESSES]) ();
int cyclesToRun[MAXIMUM_NUMBER_OF_PROCESSES];
int cyclesBetweenRuns[MAXIMUM_NUMBER_OF_PROCESSES];
int currentlyManagedProcesses = 0;
tBoolean running = false;

unsigned long uScheduler_clockRate;

void uScheduler_configure(void){
    uScheduler_clockRate = SysCtlClockGet();
    
    /*Configure/enable SysTick*/
    SysTickPeriodSet(CYCLES_BETWEEN_RUNS); 
    SysTickEnable(); 
    SysTickIntEnable();
    
    char counterText[40];
    usnprintf( counterText, 25, "clock rate: %dKhz", uScheduler_clockRate/1000 );
    uLogger_info("u_scheduler.c", counterText);
}

/**
 * systick handler
 */
void uScheduler_intHandler(void){
    int i;
    /*
     * run processes that need to run (that is, cyclesToRun[process] is less than 0)
     *   - add to those program´s counters (as they have just executed now)
     * substract CYCLES_BETWEEN_RUNS to all counters (to account for the time passes since the previous sysTick)
     */
    //Disable ints (only if using a timer for uScheduler)
    //IntMasterDisable();
        
    uLogger_debug("u_scheduler.c", "Entered uScheduler handler. Beginning processing...");
    
    //check that we are not entering abnormally 
    if(running){
       uLogger_error("u_scheduler.c", "SysTick before current processing has ended! exiting"); //this most probably does not happen, but left here just in case...
       return;
    }
    running = true;
    
    //run processes that need to run
    for(i = 0; i < currentlyManagedProcesses; i++){
        if( cyclesToRun[i] < 0 ){
            uLogger_debug("u_scheduler.c", "running a process...");
            (*processes[i]) ();
            cyclesToRun[i] = cyclesToRun[i] + cyclesBetweenRuns[i];            
            uLogger_debug("u_scheduler.c", "...ok");
        }
    }
    
    //update counters
    for(i = 0; i < currentlyManagedProcesses; i++){
        cyclesToRun[i] = cyclesToRun[i] - CYCLES_BETWEEN_RUNS;
    }
    
    running = false;
    
    uLogger_debug("u_scheduler.c", "...leaving uScheduler handler");
    

    //Reenable ints (only if using a timer)
    //IntMasterEnable();

}

void uScheduler_addProcess( void (*process)(void), int msBetweenCalls){
    uLogger_info("u_scheduler.c", "adding a new process...");
    
    cyclesBetweenRuns[currentlyManagedProcesses] = (int) ( msBetweenCalls * 1/((double)uScheduler_clockRate) );
    cyclesToRun[currentlyManagedProcesses] = cyclesBetweenRuns[currentlyManagedProcesses];
    processes[currentlyManagedProcesses] = process;
    currentlyManagedProcesses++;
    
    uLogger_info("u_scheduler.c", "...ok");
}
