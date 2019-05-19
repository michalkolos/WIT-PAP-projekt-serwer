#include <stdio.h>
#include <pthread.h>

#include "processes.h"
#include "threadpool.h"
#include "server.h"

#define THREAD_NO 4




int main(int argc, char const *argv[]){
    

    startServer(0, 9000);

    printf("Hello World!\n\n");
    printf("CPU count: %d\n\n", getCpuNumber());

    getStats();


    // pthread_t   threads[THREAD_NO];
    // int         irets[THREAD_NO];

    // for(int i = 0; i < THREAD_NO; i++){

    //     irets[i] = pthread_create( &threads[i], NULL, primeWorker, NULL);
        
    //     if(irets[i]){

    //     printf("Error - pthread_create() return code: %d\n", irets[i]);
    //     }

    // }


    // for (int i = 0; i < THREAD_NO; i++){

    //     pthread_join( threads[i], NULL);
    // }
    
    threadPollInit(30);

    printThreadPool();



    while(1){

    }


    return 0;
}
