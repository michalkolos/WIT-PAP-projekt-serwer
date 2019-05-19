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
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/syscall.h>


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
    int         command;

    pthread_mutex_t mutex;
    ThreadPool*     threadPoolPointer;
    Thread*         nextThread;

}Thread;



typedef struct ThreadPool{

    Thread* threadListHead;
    int     threadCount;
    int     workingThreadsCount;

    pthread_mutex_t mutex;

}ThreadPool;









// 8""""8                                                                                        
// 8    " e     eeeee eeeee  eeeee e        ee   e eeeee eeeee  e  eeeee eeeee  e     eeee eeeee 
// 8e     8     8  88 8   8  8   8 8        88   8 8   8 8   8  8  8   8 8   8  8     8    8   " 
// 88  ee 8e    8   8 8eee8e 8eee8 8e       88  e8 8eee8 8eee8e 8e 8eee8 8eee8e 8e    8eee 8eeee 
// 88   8 88    8   8 88   8 88  8 88        8  8  88  8 88   8 88 88  8 88   8 88    88      88 
// 88eee8 88eee 8eee8 88eee8 88  8 88eee     8ee8  88  8 88   8 88 88  8 88eee8 88eee 88ee 8ee88 
                                                                                              

// =============================================================================================




ThreadPool* threadPool;

pthread_mutex_t muteks_kolejki=PTHREAD_MUTEX_INITIALIZER;







// 8""""8                                                        
// 8    8 eeeee  eeeee eeeee eeeee eeeee e    e eeeee eeee eeeee 
// 8eeee8 8   8  8  88   8   8  88   8   8    8 8   8 8    8   " 
// 88     8eee8e 8   8   8e  8   8   8e  8eeee8 8eee8 8eee 8eeee 
// 88     88   8 8   8   88  8   8   88    88   88    88      88 
// 88     88   8 8eee8   88  8eee8   88    88   88    88ee 8ee88 
                                                              
// =============================================================

void threadPollInit(int threadNo);
Thread* spawnThread();
Thread* allocateThreadStruct(Thread* thread);
void* threadFunction(void* arg);
int getCurrentCpuNo();
void printThreadPool();



// 8""""                                             
// 8     e   e eeeee eeee eeeee e  eeeee eeeee eeeee 
// 8eeee 8   8 8   8 8  8   8   8  8  88 8   8 8   " 
// 88    8e  8 8e  8 8e     8e  8e 8   8 8e  8 8eeee 
// 88    88  8 88  8 88     88  88 8   8 88  8    88 
// 88    88ee8 88  8 88e8   88  88 8eee8 88  8 8ee88 
                                                  

// ==================================================




void threadPollInit(int threadNo){

    /// Initializing empty thread pool
    errno = 0;
    threadPool = malloc(sizeof(ThreadPool));
    
    if(threadPool == NULL){
        // TODO: threadPool creation error handling.
    }

    threadPool->workingThreadsCount = 0;
    threadPool->threadCount = 0;

    // pthread_mutex_init(&(thpool_p->thcount_lock), NULL);

    /// Creating threads
    threadPool->threadListHead = spawnThread();

    Thread* newThread = threadPool->threadListHead;
    for(int i = 0; i < threadNo -1; i++){
        newThread->nextThread = spawnThread();
        newThread = newThread->nextThread;
    }

    return;
}





Thread* spawnThread(){

    Thread* thread = malloc(sizeof(Thread));

    if(thread == NULL){
        // TODO: thread list struct creation error handling.
    }

    thread->nextThread = NULL;

    thread->threadPoolPointer   = threadPool;
    thread->command             = THREAD_COMMAND_PAUSE;
 
    errno = 0;
    int createState = pthread_create (&thread->id, NULL, threadFunction, (void*)thread);
    if(createState != 0){
        // TODO: handle thread_create errors
        perror("pthread_create:");
    }

    printf("Spawned thread %ld\n", thread->id);
    
    threadPool->threadCount++;
    return thread;
}

 


void* threadFunction(void* arg){
    Thread* threadStruct = (Thread*) arg;

    threadStruct->cpu = getCurrentCpuNo();
    threadStruct->altId = syscall(__NR_gettid);
 

    while(1){
        
        printf("Hello from %d thread!\n", threadStruct->altId);

        sleep(2);
    }

    return arg;
}


int getCurrentCpuNo(){
    
    errno = 0;
    int cpuNo = sched_getcpu();

    if(cpuNo == -1){
        // TODO: getcpu error handling
    }

    return cpuNo;
}

void printThreadPool(){
    
    Thread* nextThread = threadPool->threadListHead;
    int threadCount = 1;

    printf("\nnexthread = %ld\n\n", nextThread);

    while(nextThread != NULL){

        while(nextThread->altId == 0){
            // Waits for all threads to initialize
        }

        printf("%4d: CPUno: %1d / thread no: %ld (%d) / status: %1d / command: %1d\n",
                threadCount,
                nextThread->cpu,
                nextThread->id,
                nextThread->altId,
                nextThread->state,
                nextThread->command);
        
        nextThread = nextThread->nextThread;
        threadCount++;
    }

    printf("\n\n");
    return;
}


