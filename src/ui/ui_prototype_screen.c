#include "inc/hw_types.h"
#include "grlib/grlib.h"
#include "drivers/formike128x128x16.h"

tContext sContext;

tContext* UI_prototypeScreen_setup(void){
    
    //Initialize display hardware
    Formike128x128x16Init();
    Formike128x128x16BacklightOn();
    
    //initialize graphics context
    GrContextInit(&sContext, &g_sFormike128x128x16);
    
    return &sContext;
}
