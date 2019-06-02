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

#include "connectionqueue.h"
#include "threadpool.h"




void threadPollInit(ThreadPool* threadPool, int threadNo, int* socket, struct sockaddr_in* address){

    threadPool->serverSocket = socket;
    threadPool->serverAddress = address;
    

    threadPool->workingThreadsCount = 0;
    threadPool->threadCount = 0;

    connectionQueueInit(&threadPool->connectionQueue);

    threadPool->threadListHead = spawnThread(threadPool);

    Thread* newThread = threadPool->threadListHead;
    for(int i = 0; i < threadNo -1; i++){
        newThread->nextThread = spawnThread(threadPool);
        newThread = newThread->nextThread;
    }

    return;
}


Thread* spawnThread(ThreadPool* threadPool){

    Thread* thread = malloc(sizeof(Thread));

    if(thread == NULL){
        // TODO: thread list struct creation error handling.
    }

    thread->nextThread = NULL;

    thread->threadPoolPointer   = threadPool;
 
    errno = 0;
    int createState = pthread_create (&thread->id, NULL, threadFunction, (void*)thread);
    if(createState != 0){
        // TODO: handle thread_create errors
        perror("pthread_create:");
    }

        // TODO: Delete next line
    printf("Spawned thread %ld\n", thread->id);
    
    threadPool->threadCount++;
    return thread;
}


void* threadFunction(void* arg){
    
    /// Initialising thread
    Thread* threadStruct = (Thread*) arg;
    threadStruct->state = THREAD_STATE_INIT;
    threadStruct->cpu = getCurrentCpuNo();
    threadStruct->altId = syscall(__NR_gettid);
    
    ConnectionQueue* connectionQueue = &threadStruct->threadPoolPointer->connectionQueue;

    int incommingConnection = 0;
    
    
    /// Thread's main loop
    while(1){
        incommingConnection = connectionQueuePull(connectionQueue);
            
            printf("Thread: %d on CPU: %d recieved: %d\n", 
                threadStruct->altId,
                threadStruct->cpu,
                incommingConnection);
        // }
    }

    return arg;
}


int getCurrentCpuNo(){
    
    errno = 0;
    // int cpuNo = sched_getcpu();

    int c, s;
    s = getcpu(&c, NULL, NULL);
    int cpuNo = (s == -1) ? s : c;

    if(cpuNo == -1){
        // TODO: getcpu error handling
    }

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


