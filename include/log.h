#ifndef LOG_H
#define LOG_H



// 8""8""8                               
// 8  8  8 eeeee eeee eeeee  eeeee eeeee 
// 8e 8  8 8   8 8  8 8   8  8  88 8   " 
// 88 8  8 8eee8 8e   8eee8e 8   8 8eeee 
// 88 8  8 88  8 88   88   8 8   8    88 
// 88 8  8 88  8 88e8 88   8 8eee8 8ee88 
                                      
// ======================================


#define LOG_MESSAGE_LEN 512

#define log(...) logm(__VA_ARGS__, NULL)





// 8""""8                                                       
// 8      eeeee eeeee  e   e eeee eeeee e   e eeeee  eeee eeeee 
// 8eeeee   8   8   8  8   8 8  8   8   8   8 8   8  8    8   " 
//     88   8e  8eee8e 8e  8 8e     8e  8e  8 8eee8e 8eee 8eeee 
// e   88   88  88   8 88  8 88     88  88  8 88   8 88      88 
// 8eee88   88  88   8 88ee8 88e8   88  88ee8 88   8 88ee 8ee88 

// ============================================================

typedef struct LogQueue LogQueue;
typedef struct LogMessage LogMessage;

struct LogQueue{
    LogMessage* tail;
    LogMessage* head;

    int size; 

   	pthread_mutex_t mutex;             // Takes care of access to the queeue.
    pthread_cond_t  conditionVar;
    pthread_t       loggingThreadId;
};


struct LogMessage{
    struct timespec timestamp;
    int             level;
    char            string[LOG_MESSAGE_LEN];
    
    LogMessage*     nextMessage;
};



// 8""""8                                                        
// 8    8 eeeee  eeeee eeeee eeeee eeeee e    e eeeee eeee eeeee 
// 8eeee8 8   8  8  88   8   8  88   8   8    8 8   8 8    8   " 
// 88     8eee8e 8   8   8e  8   8   8e  8eeee8 8eee8 8eee 8eeee 
// 88     88   8 8   8   88  8   8   88    88   88    88      88 
// 88     88   8 8eee8   88  8eee8   88    88   88    88ee 8ee88 
                                                              
// =============================================================


void logQueueInit(LogQueue* queue);
void logm(LogQueue* queue, int level, const char *str, ...);
void* logThreadFunction(void* arg);
LogMessage* readFromLogQueue(LogQueue* queue);

#endif
