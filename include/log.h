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

// #define log(...) logm(__VA_ARGS__, NULL)

// Logging levels:
#define NO_LOG  0
#define FATAL   1
#define ERROR   2
#define WARNING 3
#define INFO    4
#define DEBUG   5





// 8""""8                                                       
// 8      eeeee eeeee  e   e eeee eeeee e   e eeeee  eeee eeeee 
// 8eeeee   8   8   8  8   8 8  8   8   8   8 8   8  8    8   " 
//     88   8e  8eee8e 8e  8 8e     8e  8e  8 8eee8e 8eee 8eeee 
// e   88   88  88   8 88  8 88     88  88  8 88   8 88      88 
// 8eee88   88  88   8 88ee8 88e8   88  88ee8 88   8 88ee 8ee88 

// ============================================================

typedef struct LogQueue LogQueue;
typedef struct LogMessage LogMessage;
typedef struct LogSettings LogSettings;

struct LogSettings{
    char toConsole;
    char toSyslog;
    char toLogFile;
    char toDBase;
};

struct LogQueue{
    LogMessage* tail;
    LogMessage* head;

    int size; 

   	pthread_mutex_t mutex;             // Takes care of access to the queeue.
    pthread_cond_t  conditionVar;
    pthread_t       loggingThreadId;

    LogSettings     settings;
};

struct LogMessage{
    struct timespec timestamp;
    int             level;
    int             tid;
    char            string[LOG_MESSAGE_LEN];
    
    LogMessage*     nextMessage;
};




// 8""""8                                   88   8                    
// 8    " e     eeeee eeeee  eeeee e        88   8 eeeee eeeee  eeeee 
// 8e     8     8  88 8   8  8   8 8        88  e8 8   8 8   8  8   " 
// 88  ee 8e    8   8 8eee8e 8eee8 8e       "8  8  8eee8 8eee8e 8eeee 
// 88   8 88    8   8 88   8 88  8 88        8  8  88  8 88   8    88 
// 88eee8 88eee 8eee8 88eee8 88  8 88eee     8ee8  88  8 88   8 8ee88 

// ==================================================================

LogQueue* logq;





// 8""""8                                                        
// 8    8 eeeee  eeeee eeeee eeeee eeeee e    e eeeee eeee eeeee 
// 8eeee8 8   8  8  88   8   8  88   8   8    8 8   8 8    8   " 
// 88     8eee8e 8   8   8e  8   8   8e  8eeee8 8eee8 8eee 8eeee 
// 88     88   8 8   8   88  8   8   88    88   88    88      88 
// 88     88   8 8eee8   88  8eee8   88    88   88    88ee 8ee88 
                                                              
// =============================================================


void logQueueInit(  char consoleLevel, 
                    char syslogLevel, 
                    char fileLevel, 
                    char dbaseLevel);
void logm(int level, const char *format, ...);
void* logThreadFunction(void* arg);
LogMessage* readFromLogQueue();
LogSettings getLogSettings();


#endif
