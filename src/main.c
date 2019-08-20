#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// #include <stdlib.h>

#include "threadpool.h"
#include "server.h"
#include "connectionqueue.h"
#include "log.h"
#include "messagehandlers.h"

// TODO: Delete unnecessary includes:
// #include "serialization.h"
// #include <cJSON.h>

#define THREAD_NO 5




int main(int argc, char const *argv[]){

    logQueueInit(DEBUG, DEBUG, DEBUG, DEBUG);
    messageHandlersInit();

    // cJSON* testTable = generateJsonTable(4, 
    //     "Test table", 
    //     "Column one", "int", 
    //     "Column two", "bool", 
    //     "Column three", "double", 
    //     "Column four", "string");

    // printf("\n%s\n\n", objectToSerial(testTable));

    // addRowToJsonTable(testTable, 1, 1, 3.1, "Hello!");
    // addRowToJsonTable(testTable, 2, 0, 3.2, "Hello! again");
    // addRowToJsonTable(testTable, 3, 1, 3.0, "Hello! trice");

    // printf("\n%s\n\n", objectToSerialHumanReadable(testTable));
    
    struct sockaddr_in serverAddress;
    int serverSocket = startServer(0, 9000, &serverAddress);

    ThreadPool threadPool;
    threadPollInit(&threadPool, THREAD_NO);

    // // printThreadPool(&threadPool);

    while(1){
        serverAcceptConnection(serverSocket, &(threadPool.connectionQueue));
    }

    return 0;
}


