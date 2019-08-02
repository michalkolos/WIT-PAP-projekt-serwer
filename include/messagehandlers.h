#define _GNU_SOURCE

#ifndef MESSAGEHANDLERS_H
#define MESSAGEHANDLERS_H

#include "connection.h"

#define MESSAGE_HANDLER_NO  256
#define MAX_NAME_LEN        256
#define MAX_HEADER_LEN      12          


#define HANDLER_FREE        0
#define HANDLER_USED        1

typedef struct MessageHandlers MessageHandlers;

struct MessageHandlers{
    void (*pointers[MESSAGE_HANDLER_NO]) (Message* message);
    char handlerName[MESSAGE_HANDLER_NO][MAX_NAME_LEN];
    int handlerState[MESSAGE_HANDLER_NO];
    int size;
    int free;
};


void messageHandlersInit();
void runHandler(Message* message);
void addHandler(void (*handlerPointer) (Message* message), int position, char* name);

void unknownMessageHandler(Message* message);
void errorMessageHandler(Message* message);
void testMessageHandler(Message* message);

int readFromSocket(int socket, char* buffer, int size);



#endif