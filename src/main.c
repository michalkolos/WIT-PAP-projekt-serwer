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

    LogQueue logq;
    logQueueInit(&logq, 6, 6, 6, 6);
 
    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);

    ThreadPool threadPool;
    threadPollInit(&threadPool, &logq, 40, &serverSocket, &serverAddress);

    // printThreadPool(&threadPool);

    while(1){

    }

    return 0;
}


