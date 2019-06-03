#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <netinet/in.h>

#include "connectionqueue.h"
#include "log.h"


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

// TODO: Add struct Thread description.
typedef struct Thread{
    
    pthread_t   id;
    int         altId;
    int         cpu;
    int         state;

    pthread_mutex_t mutex;
    ThreadPool*     threadPoolPointer;
    Thread*         nextThread;
    

}Thread;


// TODO: Add struct ThreadPool description.
typedef struct ThreadPool{

    Thread* threadListHead;
    int     threadCount;
    int     workingThreadsCount;

    pthread_mutex_t mutex;
    ConnectionQueue connectionQueue;
    LogQueue*       logq;

}ThreadPool;



// 8""""8                                                        
// 8    8 eeeee  eeeee eeeee eeeee eeeee e    e eeeee eeee eeeee 
// 8eeee8 8   8  8  88   8   8  88   8   8    8 8   8 8    8   " 
// 88     8eee8e 8   8   8e  8   8   8e  8eeee8 8eee8 8eee 8eeee 
// 88     88   8 8   8   88  8   8   88    88   88    88      88 
// 88     88   8 8eee8   88  8eee8   88    88   88    88ee 8ee88 
                                                              
// =============================================================




/**
 * @brief Initiates thread pool.
 * 
 * First function to run when using thread pool. It will initialise empty but 
 * allocated ThreadPool object. Job queue, that supplies running threads with 
 * socket descriptors of incoming connections, will be also initialied as empty. 
 * Finally, a specified number of new threads will be created. Those threads 
 * will handle connections suppied to them through the job queue as socket 
 * descriptors. 
 * 
 * @param threadPool Pointer to an empty ThreadPool object.
 * @param logq Pointer to initialised LogQueue object. It provides logging 
 * functionality to the entire threadpool.
 * @param threadNo  Number of threads that will be available in the pool from the
 * start. 
 */
void threadPollInit(ThreadPool* threadPool, LogQueue* logq, int threadNo);


// TODO: Finish threadPool description
/**
 * @brief Creates new worker thread.
 * 
 * 
 * 
 * @param threadPool 
 * @return Thread* 
 */
Thread* spawnThread(ThreadPool* threadPool);

/**
 * @brief Function that is runs on every thread in the thread pool.
 * 
 * This function is executed on every new worker thread inside the thread pool.
 * it consists of initialisation phase when some environmental variables for the
 * thread are set. Those are current CPU the thread is running on, as well as 
 * its PID and TID. After that comes the main, infinite loop of the thread. This is when 
 * the thread waits for a new connection on the job queue and handles it. 
 * 
 * @param arg Generic pointer that is casted to the Thread* type. It holds all
 * information about the thread as well as serves as a medium of communication
 * with other elements of the server. 
 * @return void* Unused, returns the same Thread* object that is supplied as the 
 * function argument.
 */
void* threadFunction(void* arg);

/**
 * @brief Gets the number of the current CPU.
 * 
 * It looks for the number that represents the CPU for the thread that executes 
 * this function. It does not differentiates between logical and physical cores.
 * 
 * @return int A number assigned to the current CPU by the OS. First core has
 * number 0.
 */
int getCurrentCpuNo();

// TODO: Delete printThreadPool test function.
void printThreadPool(ThreadPool* threadPool);


#endif