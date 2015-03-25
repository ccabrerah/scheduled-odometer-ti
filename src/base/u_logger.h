#ifndef U_LOGGER_H_
#define U_LOGGER_H_
#include "grlib/grlib.h"

extern void uLogger_enableLogToDisplay(tContext* _displayGraphicContext);

extern void uLogger_disableLogToDisplay(void);

extern void uLogger_enableLogToPrintf(void);

extern void uLogger_disableLogToPrintf(void);

extern void uLogger_info(char *locationInCodeReference, char *message);

extern void uLogger_error(char *locationInCodeReference, char *message);

extern void uLogger_debug(char *locationInCodeReference, char *message);

#endif /*U_LOGGER_H_*/
