#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#define BUFFER_LEN 1024

typedef struct Message Message;
typedef struct Block Block;

struct Message{
    Block*  start;
    Block*  end; 
    int     blockLen;

   	pthread_mutex_t mutex;

    int totalLen;
    int position;
};

struct Block{
    char    string [BUFFER_LEN];
    Block*  nextBlock;
};

void messageInit(Message* message, int blockLen){
    message->totalLen = 0;
    message->position = 0;
    message->blockLen = blockLen;

    Block* newBlock = malloc(sizeof(Block));
    if(newBlock == NULL){
        // TODO: Adding connection to queue malloc error handling.
    } 
    message->start = newBlock;
    message->end = newBlock;

    pthread_mutex_init(&message->mutex, NULL);

    return;
}

void messageAddNewBlock(Message* message, char* buffer, int size){
    
    

    Block* newBlock = malloc(sizeof(Block));
    if(newBlock == NULL){
        // TODO: Adding connection to queue malloc error handling.
    }

    newBlock->nextBlock = bufer;

    pthread_mutex_lock(message->mutex);

    return;
}

void messageWrite(Message* message, char* buffer, int size){

    int noOfFullBlocks = message->blockLen % size;


    for ( int i = 0; i < )
}

void readFromSocket(int socket, char* buffer, int size){

    int bytesRead = 0;
    errno = 0;
    while ( (bytesRead = read(socket, buffer, size - 1)) > 0)
    {
        buffer[bytesRead] = 0;
        printf("%s", buffer);
    }

    // if(in == -1)
    // {
        
    // }
    // else
    // {
    //     char valString[DEFAULT_STRING_LEN];
    //     snprintf(valString, DEFAULT_STRING_LEN * 2, "Recieved %d bytes", messageSize); 
    //     logMsg(INFO, valString);
    // }
     
    // for (int i = 0; i < messageLen; ++i)
    // {
    //      message[i] = ntohs(message[i]);
    // }

    printf("\n");

    return;
}

char switchBoard(char* buffer, int size){



}



"data":{[
    {   "table1": ["col01":{"name", "type"},
                "col02":{"name", "type"}]
        "table2": ["col01":{"name", "type"},
                "col02":{"name", "type"}]
    ] }