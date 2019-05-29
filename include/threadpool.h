#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <netinet/in.h>
#include "connectionqueue.h"


// 8""8""8                               
// 8  8  8 eeeee eeee eeeee  eeeee eeeee 
// 8e 8  8 8   8 8  8 8   8  8  88 8   " 
// 88 8  8 8eee8 8e   8eee8e 8   8 8eeee 
// 88 8  8 88  8 88   88   8 8   8    88 
// 88 8  8 88  8 88e8 88   8 8eee8 8ee88 
                                      
// ======================================


#define INITIAL_NUMBER_OF_THREADS 20

// definitions for thread.state
#define THREAD_STATE_DEAD       0
#define THREAD_STATE_PAUSED     1
#define THREAD_STATE_IDLE       2

#define THREAD_STATE_INIT       3
#define THREAD_STATE_WAITING    4
#define THREAD_STATE_PARSING    5
#define THREAD_STATE_DATABASE   6
#define THREAD_STATE_CLEANUP    7

// definitions for thread.command
#define THREAD_COMMAND_KILL     0
#define THREAD_COMMAND_PAUSE    1
#define THREAD_COMMAND_START    2





// 8""""8                                                       
// 8      eeeee eeeee  e   e eeee eeeee e   e eeeee  eeee eeeee 
// 8eeeee   8   8   8  8   8 8  8   8   8   8 8   8  8    8   " 
//     88   8e  8eee8e 8e  8 8e     8e  8e  8 8eee8e 8eee 8eeee 
// e   88   88  88   8 88  8 88     88  88  8 88   8 88      88 
// 8eee88   88  88   8 88ee8 88e8   88  88ee8 88   8 88ee 8ee88 

// ============================================================

typedef struct ThreadPool ThreadPool;
typedef struct Thread Thread;

typedef struct Thread{
    
    pthread_t   id;
    int         altId;
    int         cpu;
    int         state;

    pthread_mutex_t mutex;
    ThreadPool*     threadPoolPointer;
    Thread*         nextThread;
    

}Thread;



typedef struct ThreadPool{

    Thread* threadListHead;
    int     threadCount;
    int     workingThreadsCount;
    
    struct  sockaddr_in* serverAddress;
    int*    serverSocket;

    pthread_mutex_t mutex;
    ConnectionQueue connectionQueue;

}ThreadPool;



// 8""""8                                                        
// 8    8 eeeee  eeeee eeeee eeeee eeeee e    e eeeee eeee eeeee 
// 8eeee8 8   8  8  88   8   8  88   8   8    8 8   8 8    8   " 
// 88     8eee8e 8   8   8e  8   8   8e  8eeee8 8eee8 8eee 8eeee 
// 88     88   8 8   8   88  8   8   88    88   88    88      88 
// 88     88   8 8eee8   88  8eee8   88    88   88    88ee 8ee88 
                                                              
// =============================================================

void threadPollInit(ThreadPool* threadPool, int threadNo, int* socket, struct sockaddr_in* address);
Thread* spawnThread(ThreadPool* threadPool);
void* threadFunction(void* arg);
int getCurrentCpuNo();
void printThreadPool(ThreadPool* threadPool);




#endif