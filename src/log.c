#include <stdarg.h> 
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "log.h"
 
// unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    
    //log levels
    const char* logLabel[] = {
        "  [OFF]",    // 0
        "[FATAL]",    // 1
        "[ERROR]",    // 2
        " [WARN]",    // 3
        " [INFO]",    // 4
        "[DEBUG]"     // 5
    }; 


void logQueueInit(  char consoleLevel, 
                    char syslogLevel, 
                    char fileLevel, 
                    char dbaseLevel){

    logq = malloc(sizeof(LogQueue));

    int status = pthread_mutex_init(&(logq->mutex), NULL);
    if(status != 0){
        // TODO: Handle mutex initialisation error.
    }

    status = pthread_cond_init(&(logq->conditionVar), NULL);
    if(status != 0){
        // TODO: Handle condition variable initialisation error.
    }

    logq->settings.toConsole = consoleLevel;
    logq->settings.toSyslog = syslogLevel;
    logq->settings.toLogFile = fileLevel;
    logq->settings.toDBase = dbaseLevel;

    pthread_mutex_lock(&logq->mutex);
    logq->tail = NULL;
    logq->head = NULL;
    logq->size = 0;
    pthread_mutex_unlock(&logq->mutex);

    errno = 0;
    int createState = pthread_create (&logq->loggingThreadId, NULL, logThreadFunction, NULL);
    if(createState != 0){
        // TODO: handle thread_create errors
        perror("pthread_create:");
    }
    
    logm(INFO, "Logging module started with log levels set to:\n\t* console  - %s\n\t* Syslog   - %s\n\t* log file - %s\n\t* database - %s\n",
                logLabel[logq->settings.toConsole],
                logLabel[logq->settings.toSyslog],
                logLabel[logq->settings.toLogFile],
                logLabel[logq->settings.toDBase]);
 
    return;
}




void logm(int level, const char *format, ...){

    if (logq == NULL){
        logQueueInit(DEBUG, DEBUG, DEBUG, DEBUG);
        logm(WARNING, "Log queue not initialized. Automatic initialization with log level set to DEBUG.");
    }
    
    struct timespec timestamp;
    timespec_get (&timestamp, TIME_UTC);

    LogMessage* message = malloc(sizeof(LogMessage));
    if(message == NULL){
        // TODO: Adding connection to queue malloc error handling.
    }

    message->level = level;
    message->timestamp = timestamp;
    message->tid = syscall(__NR_gettid);
    message->string[0] = '\0';

    va_list arg;

    va_start(arg, format);

    vsnprintf(message->string, LOG_MESSAGE_LEN, format, arg);

    va_end(arg);

    pthread_mutex_lock(&logq->mutex);
    
    logq->size++;
    
    if (logq->head == NULL){
        logq->head = message;
        logq->tail = message;
        
        pthread_cond_signal(&logq->conditionVar);
        // TODO: Log waking up one thread from thread pool.
    }else{
        logq->tail->nextMessage = message;
        logq->tail = message;

        pthread_cond_broadcast(&logq->conditionVar);
        // TODO: Log waking up all threads from thread pool.
    }

    pthread_mutex_unlock(&logq->mutex);

    return;
}


void* logThreadFunction(void* arg){

    // TODO: Additional thread initialisation.

    LogMessage* incomingLog;
    LogSettings settings;
    while(1){
        incomingLog = readFromLogQueue();
        settings = getLogSettings();

        if(incomingLog->level <= settings.toConsole){

            struct tm timestamp = *localtime(&incomingLog->timestamp.tv_sec);

            printf("%s [%02d-%02d-%04d %02d:%02d:%02d] %s (in thread: %d)\n", 
                logLabel[incomingLog->level],
                timestamp.tm_mday,
                timestamp.tm_mon + 1,
                timestamp.tm_year + 1900,
                timestamp.tm_hour,
                timestamp.tm_min,
                timestamp.tm_sec,
                incomingLog->string,
                incomingLog->tid);
        }
        
        if(incomingLog->level <= settings.toSyslog){
            // TODO: Send log message to syslog.
        }

        if(incomingLog->level <= settings.toLogFile){
            // TODO: Write log message to file.
        }

        if(incomingLog->level <= settings.toDBase){
            // TODO: Save log message in data base.
        }
 
    }

    return arg;
}



LogMessage* readFromLogQueue(){



    LogMessage* returnMessage = NULL;

    pthread_mutex_lock(&logq->mutex);

        while (logq->size == 0) {
		pthread_cond_wait(&logq->conditionVar, &logq->mutex);
	}

    if(logq->head != NULL){
        LogMessage* oldHead = logq->head;
        logq->head = logq->head->nextMessage;

        returnMessage = oldHead;
        logq->size--;
    }
    
    if(logq->size == 0){
        logq->head = NULL;
        logq->tail = NULL;
    }

    pthread_mutex_unlock(&logq->mutex);

    return returnMessage;
}

LogSettings getLogSettings(){

    if (logq == NULL){
        logQueueInit(DEBUG, DEBUG, DEBUG, DEBUG);
        logm(WARNING, "Log queue not initialized. Automatic initialization with log level set to DEBUG.");
    }

    pthread_mutex_lock(&logq->mutex);

    LogSettings settings = logq->settings;

    pthread_mutex_unlock(&logq->mutex);

    return settings;
}

// TODO: Add function that sets log settings.