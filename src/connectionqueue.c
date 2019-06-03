#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "connectionqueue.h"
#include "log.h"


void connectionQueueInit(ConnectionQueue* queue){
    
    int status = pthread_mutex_init(&(queue->mutex), NULL);
    if(status != 0){
        logm(FATAL, "Unable to initialize connection queue mutex.");
        exit(EXIT_FAILURE);
    }

    status = pthread_cond_init(&(queue->conditionVar), NULL);
    if(status != 0){
        logm(FATAL, "Unable to initialize condition variable for connection queue.");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&queue->mutex);
    queue->tail = NULL;
    queue->head = NULL;
    queue->size = 0;
    pthread_mutex_unlock(&queue->mutex);

    return;
}

void connectionQueuePush(ConnectionQueue* queue, int socket){
    
    Connection* newConnection = malloc(sizeof(Connection));
    if(newConnection == NULL){
        logm(FATAL, "Unable to allocate memory for a new Connection struct.");
        exit(EXIT_FAILURE);
    }    
    newConnection->socket = socket;


    pthread_mutex_lock(&queue->mutex);
    
    queue->size++;
    
    if (queue->head == NULL){
        queue->head = newConnection;
        queue->tail = newConnection;
        
        pthread_cond_signal(&queue->conditionVar);
        logm(DEBUG, "Sent wake-up signal to one worker thread (connection queue size: %d).",
            queue->size);
    }else{
        queue->tail->nextConnection = newConnection;
        queue->tail = newConnection;

        pthread_cond_broadcast(&queue->conditionVar);
        logm(DEBUG, "Sent wake-up signal to all worker threads (connection queue size: %d).",
            queue->size);
    }

    pthread_mutex_unlock(&queue->mutex);

    return;
}


int connectionQueuePull(ConnectionQueue* queue){
    int socket = 0;

    pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0) {
        logm(DEBUG, "Worker waiting for new connections.");
		pthread_cond_wait(&queue->conditionVar, &queue->mutex);
	}

    if(queue->head != NULL){
        Connection* oldHead = queue->head;
        queue->head = queue->head->nextConnection;

        socket = oldHead->socket;
        free(oldHead);
        queue->size--;
        logm(DEBUG, "Worker thread received new connection: %d", socket);
    }
    
    if(queue->size == 0){
        queue->head = NULL;
        queue->tail = NULL;
    }

    pthread_mutex_unlock(&queue->mutex);

    return socket;
}

void connectionQueueWait(ConnectionQueue* queue){

    pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0) {
		pthread_cond_wait(&queue->conditionVar, &queue->mutex);
	}

    pthread_mutex_unlock(&queue->mutex);

    return;
}




void connectionQueuePrint(ConnectionQueue* queue){

    Connection* pointer;
    int len = 0;
    
    printf("Connection queue (head first)\n");

    pthread_mutex_lock(&queue->mutex);
    pointer = queue->head;
    
    while (pointer != NULL){
        printf("%d ", pointer->socket);
        len++;

        if(pointer == queue->tail){ break; }
        pointer = pointer->nextConnection; 
    }
    
    pthread_mutex_unlock(&queue->mutex);

    printf("\n Queue length: %d (measured: %d)\n", queue->size, len);
    
    return;
}
