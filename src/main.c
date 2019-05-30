#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"


#define THREAD_NO 1




int main(int argc, char const *argv[]){
    
    ThreadPool threadPool;
    threadPollInit(&threadPool, THREAD_NO);
    // printThreadPool(&threadPool);
    
    struct sockaddr_in serverAddress;
    int serverSocket = startServer(INADDR_ANY, 9000, &serverAddress);

    int connectionSocket = 0;
    while(1){

        connectionSocket = acceptConnection(serverSocket,&serverAddress);
        connectionQueuePush(&threadPool.connectionQueue, connectionSocket);
    }

    // for (int i = 0; i < 1000000; i++)
    // {
    //     connectionQueuePush(&threadPool.connectionQueue, i);
    // }
    
    // sleep(10);

    // printf("\n\n");
    // connectionQueuePrint(&threadPool.connectionQueue);
    // while(1){

    // }

    return 0;
}
