#ifndef CONNECTIONQUEUE_H
#define CONNECTIONQUEUE_H

#include <pthread.h>



typedef struct ConnectionQueue ConnectionQueue;
typedef struct Connection Connection;

struct ConnectionQueue{
    Connection* tail;
    Connection* head;

    int size; 

   	pthread_mutex_t mutex;             // Takes care of access to the queeue.
    pthread_cond_t  conditionVar;
};


struct Connection{
    int socket;
    Connection* nextConnection;
};


void connectionQueueInit(ConnectionQueue* queue);
void connectionQueuePush(ConnectionQueue* queue, int socket);
int  connectionQueuePull(ConnectionQueue* queue);
void connectionQueueWait(ConnectionQueue* queue);
void connectionQueuePrint(ConnectionQueue* queue);


#endif