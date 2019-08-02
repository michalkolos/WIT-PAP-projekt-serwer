#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// #include <stdlib.h>

#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"
#include "log.h"
#include "messagehandlers.h"

#define THREAD_NO 20




int main(int argc, char const *argv[]){

    logQueueInit(DEBUG, DEBUG, DEBUG, DEBUG);
    messageHandlersInit();

    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);

    ThreadPool threadPool;
    threadPollInit(&threadPool, THREAD_NO);

    // printThreadPool(&threadPool);

    while(1){
        serverAcceptConnection(serverSocket, &(threadPool.connectionQueue));
    }

    return 0;
}


