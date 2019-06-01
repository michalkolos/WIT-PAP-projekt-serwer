#include <stdarg.h> 
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
 




// unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
 




void logm(int level, const char *str, ...)
{

    struct timespec timestamp;
    timespec_get (&timestamp, TIME_UTC);

    LogMessage* message = malloc(sizeof(LogMessage));
    if(message == NULL){
        // TODO: Adding connection to queue malloc error handling.
    }

    message->level = level;
    message->timestamp = timestamp;
    message->string[0] = '\0';

    va_list arg;

    va_start(arg, str);

    while (str) { 
        strncat(message->string, str, LOG_MESSAGE_LEN);
        str = va_arg(arg, const char *);
    }

    va_end(arg);

    printf("%s", message->string);

    return;
}


