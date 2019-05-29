#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"


#define THREAD_NO 4




int main(int argc, char const *argv[]){
    
    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);

    printf("CPU count: %d\n\n", getCpuNumber());

    getStats();

    ThreadPool threadPool;

    threadPollInit(&threadPool, 40, &serverSocket, &serverAddress);

    printThreadPool(&threadPool);


    for (int i = 0; i < 1000000; i++)
    {
        connectionQueuePush(&threadPool.connectionQueue, i);
    }
    
    sleep(10);

    printf("\n\n");
    connectionQueuePrint(&threadPool.connectionQueue);

    return 0;
}
