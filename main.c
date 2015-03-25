//-----------------------------------------------------
//             Includes/defines
//-----------------------------------------------------

/**
 * Standard C libraries
 */
#include <string.h>
#include <stdio.h>

/**
 * Hardware related includes
 */
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

/**
 * Externally developed libraries
 */
#include "libs/ustdlib.h" //micro standard library
#include "libs/protoThreads/pt.h" //protothreads

/**
 * In-house developed libraries
 */
#include "src/base/u_scheduler.h"
#include "src/base/u_logger.h"
#include "src/base/u_clock.h"
#include "src/base/static_array_list.h"

/**
 * Peripheral-related
 */
#include "src/ui/ui_prototype_screen.h"
#include "src/capture/r_encoder/r_encoder.h"
#include "src/processing/r_encoder_data_processing.h"

/**
 * Data
 */
#include "src/data/movement_db.h"


//-----------------------------------------------------
//                     Macros
//-----------------------------------------------------
#define MAX_PROTOTHREADS 20

//-----------------------------------------------------
//         Initialize memory/prepare state
//-----------------------------------------------------
/**
 * Protothreads-related 
 */
static unsigned int protothreads_running = 0;
struct pt protothreads_state[MAX_PROTOTHREADS];
static int (*protothreads[MAX_PROTOTHREADS]) (struct pt*);

/**
 * others
 */
volatile int printVelocityCounter = 0;
char textToDisplay[60];
volatile int lastVelocity;
//-----------------------------------------------------
//                  Functions
//----------------------------------------------------- 
/**
 * Really basic setup, e.g. main processor clock.
 */
void basicHardwareSetup(void){
    long delay = 1000000;
    
    //setup clock at 50Mhz (20Mhz?) using the PLL (using the external 8Mhz crystal)
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    
    
    //delay for a while...  
    while(delay != 0){
        delay--;
    }
}

/**
 * Defines interrupts priorities.
 */
void configInterruptsPriorities(void){
    /**
     * 0x00,0x20... 0x00 is the highest priority level, and it is the default one too
     * SysTick has to be handled separately
     */
    
    IntPrioritySet(INT_TIMER0A, 0x00);
    
}
 
/**
 * printVelocity protothread
 */
int printVelocityProtothread(struct pt* pt){
    PT_BEGIN( pt );
    
    while(1) {
        PT_WAIT_UNTIL(pt, printVelocityCounter++ && ((printVelocityCounter % 50000) == 12));
        lastVelocity = (int) mdb_l1_getLastKnownVelocityByIndex(0);
        usnprintf( textToDisplay, 50, "Velocity %d: %d", printVelocityCounter/50000, lastVelocity);
        uLogger_info("main.c", textToDisplay);
    }  
    
    PT_END(pt);
}

/**
 * Main method.
 * - calcular periodicamente nueva informacion
 * - mostrar informacion en alguna parte
 * + generar informacion para pruebas 'asincronamente'
 * 
 * procesos/cooperative multitasking:
 *  -> "retornar si no hay nada que hacer"
 *  -> "retornar si hay que esperar" -> no despertar hasta que haya "pasado una cierta cantidad de tiempo" (o derechamente monitores?)
 *  -> poder asignar prioridad a 'procesos'/controlar la frecuencia con que despiertan
 */
int main(void){
    int i;
    
    //doSomethingWithList();
  
    /**
     * Basic configuration
     */
    basicHardwareSetup();
    configInterruptsPriorities();
    uLogger_enableLogToPrintf(); //TODO: fix CIO problems
    uLogger_info("main.c", "basic hardware and logging configuration is complete");
    
    uLogger_info("main.c", "starting uScheduler...");
    uScheduler_configure();
    uLogger_info("main.c", "...ok");
    
    uLogger_info("main.c", "starting uClock...");
    uClock_initialize();
    uLogger_info("main.c", "...ok");
  
    /**
     * Configure connected peripherals
     */
    tContext* graphicsContext = UI_prototypeScreen_setup();
    //uLogger_enableLogToDisplay(graphicsContext);
        
    /**
     * Setup periodic processes
     */
    REncoder_config();
    
//   	Capture_REncoder_config( assignedPortInfo, encoderType, ? ); 
//   	Capture_REncoder_enableCapture();
//   	uScheduler_addProcess( &DCapture_REncoder_doAggregation, 1, 1 );
 		
//   	Capture_Accelerometer_config( assignedPortInfo, type, ? );
//   	Capture_Accelerometer_enableCapture();
//   	uScheduler_addProcess( &DCapture_Accelerometer_doAggregation, 1, 1 );

    /**
     * Install protothreads
     */
    //printVelocity
    //PT_INIT( &protothreads_state[protothreads_running] );
    //protothreads[protothreads_running++] = printVelocityProtothread;
    //REncoder information processing
    PT_INIT( &protothreads_state[protothreads_running] );
    protothreads[protothreads_running++] = REncoder_periodicProcessingProtoThread;
    
    /**
     * Misc. configuration
     */

    /**
     * Protothread main loop
     */
    while(1){
        for(i=0; i<protothreads_running; i++){
            (*protothreads[i]) ( &protothreads_state[i]);
        }
    }
}
