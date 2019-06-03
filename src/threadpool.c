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
#include "log.h"




void threadPollInit(ThreadPool* threadPool, LogQueue* logq, int threadNo){

    threadPool->logq = logq;

    threadPool->workingThreadsCount = 0;
    threadPool->threadCount = 0;

    connectionQueueInit(&threadPool->connectionQueue, logq);

    threadPool->threadListHead = spawnThread(threadPool);
    if(threadPool->threadListHead == NULL){
        logm(logq, FATAL, "Error creating worker thread pool.");
        exit(EXIT_FAILURE);
    }

    Thread* newThread = threadPool->threadListHead;
    for(int i = 0; i < threadNo -1; i++){
        newThread->nextThread = spawnThread(threadPool);
        newThread = newThread->nextThread;
    }

    if(threadPool->threadCount <= 0){
        logm(logq, FATAL, "Error creating worker thread pool.");
        exit(EXIT_FAILURE);
    }
    if(threadPool->threadCount != threadNo){
        logm(logq, WARNING, "Amount of created worker threads different than ordered: %d.", 
            threadPool->threadCount);
    }

    if(threadPool->threadCount == threadNo){
        logm(logq, INFO, "Created worker thread pool of %d threads.", 
            threadPool->threadCount);
    }

    return;
}


Thread* spawnThread(ThreadPool* threadPool){
    LogQueue* logq = threadPool->logq;

    Thread* thread = malloc(sizeof(Thread));

    if(thread == NULL){
        logm(logq, FATAL, "Error creating worker thread struct.");
        exit(EXIT_FAILURE);
    }

    thread->nextThread = NULL;

    thread->threadPoolPointer   = threadPool;
 
    errno = 0;
    int createState = pthread_create (&thread->id, NULL, threadFunction, (void*)thread);
    if(createState != 0){
        logm(logq, ERROR, "Error spawning thread | %s", strerror(errno));
        free(thread);
        return NULL;
    }

    logm(logq, DEBUG, "Spawned thread %ld.", thread->id);
    
    threadPool->threadCount++;
    return thread;
}


void* threadFunction(void* arg){
    
    /// Initialising thread
    Thread* threadStruct = (Thread*) arg;

    ConnectionQueue* connectionQueue = &threadStruct->threadPoolPointer->connectionQueue;
    LogQueue* logq = threadStruct->threadPoolPointer->logq; 
    
    threadStruct->state = THREAD_STATE_INIT;
    
    /// Checking id of the CPU that is running the thread
    threadStruct->cpu = getCurrentCpuNo();
    if(threadStruct->cpu == -1){
        logm(logq, ERROR, "Error reading CPU id | %s", strerror(errno));
    }

    /// Geting TID of the thread
    errno = 0;
    threadStruct->altId = syscall(__NR_gettid);
    if(threadStruct->altId == -1){
        logm(logq, ERROR, "Error reading thread TID | %s", strerror(errno));
    }

    int incomingConnection = 0;
    
    logm(logq, DEBUG, "%s", "Worker thread ready.");

    /// Thread's main loop
    while(1){
        incomingConnection = connectionQueuePull(connectionQueue);
            
            printf("Thread: %d on CPU: %d recieved: %d\n", 
                threadStruct->altId,
                threadStruct->cpu,
                incomingConnection);
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


