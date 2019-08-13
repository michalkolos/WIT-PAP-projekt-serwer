#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <sys/prctl.h>
#include <malloc.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <string.h>

#include "connectionqueue.h"
#include "threadpool.h"
#include "connection.h"
#include "log.h"


// TODO: Add capability for creating and destroying additional threads. 


void threadPollInit(ThreadPool* threadPool, int threadNo){

    threadPool->workingThreadsCount = 0;
    threadPool->threadCount = 0;

    connectionQueueInit(&threadPool->connectionQueue);

    threadPool->threadListHead = spawnThread(threadPool);
    if(threadPool->threadListHead == NULL){
        logm(FATAL, "Error creating worker thread pool.");
        exit(EXIT_FAILURE);
    }

    Thread* newThread = threadPool->threadListHead;
    for(int i = 0; i < threadNo -1; i++){
        newThread->nextThread = spawnThread(threadPool);
        newThread = newThread->nextThread;
    }

    if(threadPool->threadCount <= 0){
        logm(FATAL, "Error creating worker thread pool.");
        exit(EXIT_FAILURE);
    }
    if(threadPool->threadCount != threadNo){
        logm(WARNING, "Amount of created worker threads different than ordered: %d.", 
            threadPool->threadCount);
    }

    if(threadPool->threadCount == threadNo){
        logm(INFO, "Created worker thread pool of %d threads.", 
            threadPool->threadCount);
    }

    return;
}

// TODO: Modify spawnThread function so it will put them directly into ThreadPool object instead returning it.
Thread* spawnThread(ThreadPool* threadPool){

    Thread* thread = malloc(sizeof(Thread));
    if(thread == NULL){
        logm(FATAL, "Error creating worker thread struct.");
        exit(EXIT_FAILURE);
    }

    thread->nextThread = NULL;

    thread->threadPoolPointer   = threadPool;
 
    errno = 0;
    int createState = pthread_create (&thread->id, NULL, threadFunction, (void*)thread);
    if(createState != 0){
        logm(ERROR, "Error spawning thread | %s", strerror(errno));
        free(thread);
        return NULL;
    }

    logm(DEBUG, "Spawned thread %ld.", thread->id);
    
    threadPool->threadCount++;
    return thread;
}


void* threadFunction(void* arg){
    
    /// Initialising thread
    Thread* threadStruct = (Thread*) arg;

    ConnectionQueue* connectionQueue = &threadStruct->threadPoolPointer->connectionQueue;
    
    threadStruct->state = THREAD_STATE_INIT;
    
    /// Checking id of the CPU that is running the thread
    threadStruct->cpu = getCurrentCpuNo();
    if(threadStruct->cpu == -1){
        logm(ERROR, "Error reading CPU id | %s", strerror(errno));
    }

    /// Geting TID of the thread
    errno = 0;
    threadStruct->altId = syscall(__NR_gettid);
    if(threadStruct->altId == -1){
        logm(ERROR, "Error reading thread TID | %s", strerror(errno));
    }

    int incomingConnection = 0;
    
    logm(DEBUG, "Worker thread ready.");

    
    
    /// Thread's main loop
    int totalBytesFromConnection;
    
    while(1){
        totalBytesFromConnection = 0;
        // incomingConnection = connectionQueuePull(connectionQueue);

        Message message;
        message.readSocket = connectionQueuePull(connectionQueue);

        readMessageFromSocket(&message);
        // messageFinish(&message);
    }

    return arg;
}


int getCurrentCpuNo(){
    
    errno = 0;
    // int cpuNo = sched_getcpu();

    int c, s;
    s = getcpu(&c, NULL, NULL);
    int cpuNo = (s == -1) ? s : c;

    return cpuNo;
}


void printThreadPool(ThreadPool* threadPool){
    
    Thread* nextThread = threadPool->threadListHead;
    int threadCount = 1;

    printf("\nnexthread = %ld\n\n", nextThread);

    while(nextThread != NULL){

        while(nextThread->altId == 0){
            // Waits for all threads to initialize
        }

        printf("%4d: CPUno: %1d / thread no: %ld (%d) / status: %1d\n",
                threadCount,
                nextThread->cpu,
                nextThread->id,
                nextThread->altId,
                nextThread->state);
        
        nextThread = nextThread->nextThread;
        threadCount++;
    }

    printf("\n\n");
    return;
}


