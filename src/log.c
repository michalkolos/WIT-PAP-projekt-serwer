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
 






void logQueueInit(LogQueue* queue,  char consoleLevel, 
                                    char syslogLevel, 
                                    char fileLevel, 
                                    char dbaseLevel){

    int status = pthread_mutex_init(&(queue->mutex), NULL);
    if(status != 0){
        // TODO: Handle mutex initialisation error.
    }

    status = pthread_cond_init(&(queue->conditionVar), NULL);
    if(status != 0){
        // TODO: Handle condition variable initialisation error.
    }

    queue->toConsole = consoleLevel;
    queue->toSyslog = syslogLevel;
    queue->toLogFile = fileLevel;
    queue->toDBase = dbaseLevel;

    pthread_mutex_lock(&queue->mutex);
    queue->tail = NULL;
    queue->head = NULL;
    queue->size = 0;
    pthread_mutex_unlock(&queue->mutex);

    errno = 0;
    int createState = pthread_create (&queue->loggingThreadId, NULL, logThreadFunction, (void*)queue);
    if(createState != 0){
        // TODO: handle thread_create errors
        perror("pthread_create:");
    }
 
    return;
}




void logm(LogQueue* queue, int level, const char *str, ...){

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

    va_start(arg, str);

    while (str) { 
        strncat(message->string, str, LOG_MESSAGE_LEN);
        str = va_arg(arg, const char *);
    }

    va_end(arg);

    // printf("%s", message->string);

    pthread_mutex_lock(&queue->mutex);
    
    queue->size++;
    
    if (queue->head == NULL){
        queue->head = message;
        queue->tail = message;
        
        pthread_cond_signal(&queue->conditionVar);
        // TODO: Log waking up one thread from thread pool.
    }else{
        queue->tail->nextMessage = message;
        queue->tail = message;

        pthread_cond_broadcast(&queue->conditionVar);
        // TODO: Log waking up all threads from thread pool.
    }

    pthread_mutex_unlock(&queue->mutex);

    return;
}


void* logThreadFunction(void* arg){

    LogQueue* queue = (LogQueue*) arg;
    // TODO: Additional thread initialisation.

    //log levels
    const char* logLabel[] = {
        "  [ALL]",    // 0
        "[FATAL]",    // 1
        "[ERROR]",    // 2
        " [WARN]",    // 3
        " [INFO]",    // 4
        "[DEBUG]"     // 5
    };




    LogMessage* incomingLog;
    while(1){
        incomingLog = readFromLogQueue(queue);
 

        if(incomingLog->level <= queue->toConsole){

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
        
    // TODO: Parse time in timestamps of log messages.

    // TODO: Handle message:
    // TODO: Send log message to syslog.
    // TODO: Write log message to file.
    // TODO: Save log message in data base.
    // 
 
    }

    return arg;
}



LogMessage* readFromLogQueue(LogQueue* queue){

    LogMessage* returnMessage = NULL;

    pthread_mutex_lock(&queue->mutex);

        while (queue->size == 0) {
		pthread_cond_wait(&queue->conditionVar, &queue->mutex);
	}

    if(queue->head != NULL){
        LogMessage* oldHead = queue->head;
        queue->head = queue->head->nextMessage;

        returnMessage = oldHead;
        queue->size--;
    }
    
    if(queue->size == 0){
        queue->head = NULL;
        queue->tail = NULL;
    }

    pthread_mutex_unlock(&queue->mutex);

    return returnMessage;
}


const char* my_itoa(int num){

    static char buf[sizeof(int) * 8];
    int len = sizeof(buf);

    if (snprintf(buf, len, "%d", num) == -1){
        return "";
    }

  return buf;
}