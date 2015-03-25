#include <string.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "../../libs/ustdlib.h"
#include "synchronization.h"
#include "grlib/grlib.h"

#define DEBUG_ENABLED false

tBoolean printfActive = false;

tBoolean displayActive = false;
tContext* displayGraphicContext; //graphic context; it is expected that it has been initialized before

volatile unsigned int currentLogLine = 0;

volatile unsigned int displayLock = 0;
volatile unsigned int printfLock = 0;


void uLogger_enableLogToPrintf(void){
    printfActive = true;
}

void uLogger_disableLogToPrintf(void){
    printfActive = false;
}

void uLogger_enableLogToDisplay(tContext* _displayGraphicContext){
    displayGraphicContext = _displayGraphicContext;
    displayActive = true;
}

void uLogger_disableLogToDisplay(void){
    displayGraphicContext = 0;
    displayActive = false;
}

void out(char *locationInCodeReference, char* levelString, char *text){
    if(printfActive){
    	if(sync_tryLock(&printfLock)){
	    	IntMasterDisable();
	        printf(locationInCodeReference);
	        printf(" ");
	        printf(levelString);
	        printf(text);
	        printf("\r\n");
	        IntMasterEnable();
	        sync_releaseLock(&printfLock);
    	}
    }
    
    if(displayActive){
    	if(sync_tryLock(&displayLock)){
	        //TODO: implement scrolling/preserve previous messages
	        tRectangle sRect;
	        
	        //1 repaint display with one color
	        sRect.sXMin = 0;
	        sRect.sYMin = 0;
	        sRect.sXMax = GrContextDpyWidthGet(displayGraphicContext) - 1;
	        sRect.sYMax = GrContextDpyHeightGet(displayGraphicContext) / 2 - 1;
	        GrContextForegroundSet(displayGraphicContext, ClrSteelBlue);
	        GrRectFill(displayGraphicContext, &sRect);
	        
	        //2 paint text
	        GrContextForegroundSet(displayGraphicContext, ClrWhite);
	        GrContextFontSet(displayGraphicContext, &g_sFontCm12);
	        
	        GrStringDraw(displayGraphicContext, locationInCodeReference, -1, 0, 0, false);
	        GrStringDraw(displayGraphicContext, levelString, -1, 8*strlen(locationInCodeReference), 0, false);
	        
	        GrStringDraw(displayGraphicContext, text, -1, 0, 12, false);
	        if(strlen(text)>22) GrStringDraw(displayGraphicContext, (text+22), -1, 0, 24, false);
	        if(strlen(text)>44) GrStringDraw(displayGraphicContext, (text+44), -1, 0, 36, false);
	        if(strlen(text)>66) GrStringDraw(displayGraphicContext, (text+66), -1, 0, 48, false);    
	                             
	        GrFlush(displayGraphicContext);
	        
	        sync_releaseLock(&displayLock);
    	}
        
    }
}

void uLogger_info(char *locationInCodeReference, char *message){
    out(locationInCodeReference, "INFO - ", message);
}

void uLogger_error(char *locationInCodeReference, char *message){
    out(locationInCodeReference, "ERROR - ", message);
}

void uLogger_debug(char *locationInCodeReference, char *message){
    if( DEBUG_ENABLED ) {
        out(locationInCodeReference, "DEBUG - ", message);
    }
}
