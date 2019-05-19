#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
// #include <sched.h>
#include <errno.h>

// int getCurrentCpuNo(){
    
//     errno = 0;
//     int cpuNo = sched_getcpu();

//     if(cpuNo == -1){
//         // TODO: getcpu error handling
//     }

//     return cpuNo;
// }


int getCpuNumber(){

    return get_nprocs();
}

void getStats(){

    pid_t threadId          = syscall(__NR_gettid);
    pthread_t threadId02    = pthread_self();
    pid_t processId         = getpid();


    printf( "CPU: %d thread: %d thread02: %ld process: %d \n", 
            0,
            threadId, 
            (int)threadId02,
            processId);

    return;
}

void primeGenerator(int noOfPrimes){

    int i = 3;
    int c;
   
    for (int count = 2 ; count <= noOfPrimes ;  ){
        for (c = 2 ; c <= i - 1 ; c++ ){
            if ( i % c == 0 )
                break;
        }
        if ( c == i ){
            // getStats();
            // printf("%d\n", i);
            count++;
        }
        i++;
    }
 
    return;
}

void *primeWorker( void *ptr ){
    while(1){
        getStats();
        primeGenerator(5000);
    }
}


