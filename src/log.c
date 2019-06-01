#include <stdarg.h> 
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// #define log(...) logm(INT, __VA_ARGS__, NULL)


#define LOG_MESSAGE_LEN 512

typedef struct LogQueue LogQueue;
typedef struct LogMessage LogMessage;

struct LogQueue{
    LogMessage* tail;
    LogMessage* head;

    int size; 

   	pthread_mutex_t mutex;             // Takes care of access to the queeue.
    pthread_cond_t  conditionVar;
};


struct LogMessage{
    struct timespec timestamp;
    int             level;
    char            string[LOG_MESSAGE_LEN];
    
    LogMessage*     nextMessage;
};

// unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;

/**
 * @brief 
 * 
 * @param message 
 * @param ... 
 */

void logm(int level, const char *str, ...)
{
    va_list arg;

    va_start(arg, str);

    // printf("%s", va_arg(arg, const char *));
    // printf("%s", va_arg(arg, const char *));
    while (str) {
        
        // printf("%s", va_arg(arg, const char *));
 
        while (str) {
        printf("%s", str);
        str = va_arg(arg, const char *);
    }
 
    }

    va_end(arg);

    return;
}

// void log(char* message, ...);


// void log(int level, int count, ...){

//     struct timespec timestamp;
//     timespec_get (&timestamp, TIME_UTC);

//     // char *strncat(char *dest, const char *src, size_t n);

//     LogMessage* message = malloc(sizeof(LogMessage));
//     if(message == NULL){
//         // TODO: Adding connection to queue malloc error handling.
//     }

//     message->string[0] = '\0';

//     va_list valist; 
//     va_start(valist, count);

//     va_arg(valist, char[LOG_MESSAGE_LEN]); 
  
//     va_end(valist); 

//     return;
// }

  
// void log(char* string, ...) {

//     struct timespec timestamp;
//     timespec_get (&timestamp, TIME_UTC);
    
//     int i = 0; 
//     while( string[i] != '\0' ){
//         if( string[i] == '%'){ i++; }
//     }

//     va_list valist; 
//     va_start(valist, i);

//     i = 0;
//     while( string[i] != '\0' ){

        

//         i++;
//     }
//     va_arg(valist, int); 
  
//     va_end(valist); 
  
//     return;
// }