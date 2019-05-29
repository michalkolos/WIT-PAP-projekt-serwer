#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "processes.h"
#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"


#define THREAD_NO 4




int main(int argc, char const *argv[]){
    
    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);

    printf("Hello World!\n\n");
    printf("CPU count: %d\n\n", getCpuNumber());

    getStats();


    // pthread_t   threads[THREAD_NO];
    // int         irets[THREAD_NO];

    // for(int i = 0; i < THREAD_NO; i++){

    //     irets[i] = pthread_create( &threads[i], NULL, primeWorker, NULL);
        
    //     if(irets[i]){

    //     printf("Error - pthread_create() return code: %d\n", irets[i]);
    //     }

    // }


    // for (int i = 0; i < THREAD_NO; i++){

    //     pthread_join( threads[i], NULL);
    // }
    // 

    // ConnectionQueue connectionQueue;
    ThreadPool threadPool;
    // ConnectionQueue connectionQueue;

    threadPollInit(&threadPool, 40, &serverSocket, &serverAddress);
    // connectionQueueInit(&threadPool.connectionQueue);

    printThreadPool(&threadPool);

    // sleep(2);

    for (int i = 0; i < 1000000; i++)
    {
        connectionQueuePush(&threadPool.connectionQueue, i);
        // printf("%d",connectionQueuePull(&threadPool.connectionQueue));
        // connectionQueuePrint(&threadPool.connectionQueue);
    }
    
    sleep(10);

    printf("\n\n");
    connectionQueuePrint(&threadPool.connectionQueue);

    
    
    
    // ConnectionQueue queue;
    // connectionQueueInit(&queue);

    // connectionQueuePrint(&queue);

    // for(int i = 0; i < 10; i++){
    //     connectionQueuePush(&queue, i + 123124);
    // }

    // printf("\n\nPushing queue:\n");
    // connectionQueuePush(&queue, 1234);
    

    // printf("\n\nPrinting queue:\n");
    // connectionQueuePrint(&queue);
    
    // for(int i = 0; i < 10; i++){
    //     printf("%d-",connectionQueuePull(&queue));
    // }


    // printf("\n\nValue form queue: %d", connectionQueuePull(&queue));
    // printf("\n\nValue form queue: %d", connectionQueuePull(&queue));
    
    // printf("\n\nPrinting queue:\n");
    // connectionQueuePrint(&queue);

    return 0;
}
