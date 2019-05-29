#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "connectionqueue.h"

void connectionQueueInit(ConnectionQueue* queue){
    
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

    return;
}

void connectionQueuePush(ConnectionQueue* queue, int socket){
    
    Connection* newConneciton = malloc(sizeof(Connection));
    if(newConneciton == NULL){
        // TODO: Adding connection to queue malloc error handling.
    }    
    newConneciton->socket = socket;


    pthread_mutex_lock(&queue->mutex);
    
    queue->size++;
    
    if (queue->head == NULL){
        queue->head = newConneciton;
        queue->tail = newConneciton;
        
        pthread_cond_signal(&queue->conditionVar);
        // TODO: Log waking up one thread from thread pool.
    }else{
        queue->tail->nextConnection = newConneciton;
        queue->tail = newConneciton;

        pthread_cond_broadcast(&queue->conditionVar);
        // TODO: Log waking up all threads from thread pool.
    }

    pthread_mutex_unlock(&queue->mutex);

    return;
}


int connectionQueuePull(ConnectionQueue* queue){
    int socket = 0;

    pthread_mutex_lock(&queue->mutex);

        while (queue->size == 0) {
		pthread_cond_wait(&queue->conditionVar, &queue->mutex);
	}

    if(queue->head != NULL){
        Connection* oldHead = queue->head;
        queue->head = queue->head->nextConnection;

        socket = oldHead->socket;
        free(oldHead);
        queue->size--;
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
