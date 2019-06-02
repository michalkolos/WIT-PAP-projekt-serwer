#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// #include <stdlib.h>

#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"
#include "log.h"

#define THREAD_NO 20




int main(int argc, char const *argv[]){

    LogQueue queue;
    
    logQueueInit(&queue, 5, 5, 5, 5);


    for(int i = 0; i < 1000000; i++){
    
    char buffer [1024];

    log(&queue, 4, "%s %s %s %s %s %s %05d", "this ", "is ", "a ", "new ", "log ", "entry.", i);


    }
 


    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);


    ThreadPool threadPool;

    threadPollInit(&threadPool, 40, &serverSocket, &serverAddress);

    printThreadPool(&threadPool);


    for (int i = 0; i < 1000000; i++)
    {
        connectionQueuePush(&threadPool.connectionQueue, i);
    }
    
    // sleep(10);

    // printf("\n\n");
    // connectionQueuePrint(&threadPool.connectionQueue);

    while(1){

    }

    return 0;
}


