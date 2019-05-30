#ifndef CONNECTIONQUEUE_H
#define CONNECTIONQUEUE_H

#include <pthread.h>



typedef struct ConnectionQueue ConnectionQueue;
typedef struct Connection Connection;

// TODO: Add struct ConnectionQueue description.
struct ConnectionQueue{
    Connection* tail;
    Connection* head;

    int size; 

   	pthread_mutex_t mutex;             // Takes care of access to the queeue.
    pthread_cond_t  conditionVar;
};

// TODO: Add struct Connection description.
struct Connection{
    int socket;
    Connection* nextConnection;
};

/**
 * @brief Initialize job queue for connections.
 * 
 * Initializes new empty job queue for incoming connections.  
 * 
 * @param queue A pointer to uninitialized ConnectionQueue object.
 */
void connectionQueueInit(ConnectionQueue* queue);

/**
 * @brief 
 * 
 * @param queue 
 * @param socket 
 */
void connectionQueuePush(ConnectionQueue* queue, int socket);
int  connectionQueuePull(ConnectionQueue* queue);
void connectionQueueWait(ConnectionQueue* queue);
void connectionQueuePrint(ConnectionQueue* queue);


#endif