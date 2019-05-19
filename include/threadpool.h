#ifndef THREADPOOL_H
#define THREADPOOL_H

typedef struct ThreadPool ThreadPool;
typedef struct Thread Thread;

typedef struct Thread{
    
    pthread_t   id;
    int         cpu;
    int         state;
    int         command;

    ThreadPool*  threadPoolPointer;

    Thread*      nextThread;

}Thread;



typedef struct ThreadPool{

    Thread* threadList;
    int     threadCount;
    int     workingThreadsCount;

}ThreadPool;




void threadPollInit(int threadNo);
void spawnThread(Thread* thread);
void* threadFunction(void* arg);
int getCurrentCpuNo();
void printThreadPool();


#endif