#include <stdarg.h> 
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "log.h"
 
// unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
 
void logQueueInit(LogQueue* queue);
void* logThreadFunction(void* arg);






void logQueueInit(LogQueue* queue){

    int status = pthread_mutex_init(&(queue->mutex), NULL);
    if(status != 0){
        // TODO: Handle mutex initialisation error.
    }

    status = pthread_cond_init(&(queue->conditionVar), NULL);
    if(status != 0){
        // TODO: Handle condition variable initialisation error.
    }

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
    message->string[0] = '\0';

    va_list arg;

    va_start(arg, str);

    while (str) { 
        strncat(message->string, str, LOG_MESSAGE_LEN);
        str = va_arg(arg, const char *);
    }

    va_end(arg);

    printf("%s", message->string);

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


