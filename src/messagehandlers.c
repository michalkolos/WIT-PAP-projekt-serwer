#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <arpa/inet.h>

#include "messagehandlers.h"
#include "connection.h"
#include "log.h"


MessageHandlers* messageHandlers;



void messageHandlersInit(){

    messageHandlers = malloc(sizeof(MessageHandlers));
    if(messageHandlers == NULL){
        logm(FATAL, "Unable to allocate memory for a new Connection struct.");
        exit(EXIT_FAILURE);
    } 

    messageHandlers->size = MESSAGE_HANDLER_NO;
    messageHandlers->free = MESSAGE_HANDLER_NO;

    addHandler(errorMessageHandler, 0, "Error");

    for(int i = 1; i < MESSAGE_HANDLER_NO - 1; i++){
        messageHandlers->pointers[i] = unknownMessageHandler;
        messageHandlers->handlerState[i] = 0;
    } 

    addHandler(testMessageHandler, '1', "test");

    char logMessage[LOG_MESSAGE_LEN];
    char concatMessage[LOG_MESSAGE_LEN];
    snprintf(logMessage, LOG_MESSAGE_LEN, 
        "Initialised message handler table. Server can accept messages:\n");

    for (int i = 1; i < MESSAGE_HANDLER_NO; i++){
        if(messageHandlers->handlerState[i] == 1){
            snprintf(concatMessage, LOG_MESSAGE_LEN, 
                "\t* code: %3d - %s\n",
                i, 
                &messageHandlers->handlerName[i][0]);

            strcat(logMessage, concatMessage);
        }
        
    }
    snprintf(concatMessage, LOG_MESSAGE_LEN, 
        "Free spots for additional handlers: %d",
        messageHandlers->free);

        strcat(logMessage, concatMessage);
    
    logm(INFO, logMessage);

    return;
}


void runHandler(Message* message){

    return (messageHandlers->pointers[message->protocolId])(message);
}


void addHandler(void (*handlerPointer) (Message* message), int position, char* name){

    if(position >= MESSAGE_HANDLER_NO || messageHandlers->handlerState[position] != 0){
        logm(ERROR, "Unable to add new message handler.");
        return;
    }
    
    snprintf(&messageHandlers->handlerName[position][0], LOG_MESSAGE_LEN, name);

    messageHandlers->pointers[position] = handlerPointer;
    messageHandlers->handlerState[position] = 1;
    messageHandlers->free--;

    return;
}






int readFromSocket(int socket, char* buffer, int size){

    int totalBytesRead = 0;
    int bytesRead = 0;
    
    errno = 0;
    while ( (bytesRead = read(socket, buffer, size - 1)) > 0){

        totalBytesRead += bytesRead;
        buffer[bytesRead] = 0;
        printf("%s", buffer);
    }

    // TODO: Read from socket error handling.

    printf("\n");

    return totalBytesRead;
}

void setHeaderField(int field,uint8_t* buffer, int* offset, int errorField){

    uint32_t value;
    memcpy(&value, buffer + *offset, sizeof(uint32_t));

    *offset += sizeof(uint32_t);

    field = ntohl(value);

    if(field == 0 ){
        errorField = IN_HEADER_ERROR;
    }

    return;
}

void readMessageHeader(Message* message){
    int bytesRead = 0;
    int offset = 0;
    uint8_t buffer[MAX_HEADER_LEN];

    errno = 0;
    bytesRead = read(message->readSocket, buffer, MAX_HEADER_LEN);
    if(bytesRead == 0){
        logm(ERROR, "Error reading header from socket: %s", strerror(errno));
        message->error = READ_ERROR;
    }

    setHeaderField(message->clientId, buffer, &offset, message->error);
    setHeaderField(message->dataSize, buffer, &offset, message->error);

    return;
}

void readMessageString(Message* message){
    int bytesRead = 0;

    message->data = malloc(sizeof(uint8_t) * message->dataSize);
    if(message->data == NULL){
        logm(FATAL, "Error allocating memory for incoming data.");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    bytesRead = read(message->readSocket, message->data, message->dataSize);
    if(bytesRead == 0 && errno != 0){
        logm(ERROR, "Error reading data from socket: %s", strerror(errno));
        message->error = READ_ERROR;
    }    

    if(message->data[message->dataSize - 1] != '\0'){
        message->error = IN_DATA_ERROR;
    }

    message->bytesReceived += bytesRead;

    return;
}


void unknownMessageHandler(Message* message){

    logm(ERROR, "Unknown message format");
    // TODO: Implement unknownMessageHandler.
    return;
}

void errorMessageHandler(Message* message){

    logm(ERROR, "Received bad message.");
    // TODO: Implement errorMessageHandler.
    return;
}

void testMessageHandler(Message* message){

    readMessageHeader(message);
    readMessageString(message);   
    // char readBuffer[BUFFER_LEN];
    // readFromSocket(message->readSocket, readBuffer, BUFFER_LEN);
    // // TODO: Implement testMessageHandler.
    
    
    
    return;
}

// uint32_t parseInt(char* buffer){
//     uint32_t value = (uint32_t)buffer[0];
//     value = ntohl(value);

//     return value;
// }

// void readHeader(int socket, Message* message){

//     char buffer[BUFFER_LEN];
//     message->bytesReceived += readFromSocket(socket, buffer, BUFFER_LEN);

//     int index = 0;

//     message->protocolId = parseInt(buffer + index);
//     index += sizeof(uint32_t);

//     message->clientId = parseInt(buffer + index);
//     index += sizeof(uint32_t);

//     message->crc = parseInt(buffer + index);
//     index += sizeof(uint32_t);

//     message->dataSize = parseInt(buffer + index);
//     index += sizeof(uint32_t);

//     logm(DEBUG, "Received header: protocol: %d//client: %d//crc: %d//data size: %d",
//         message->protocolId,
//         message->clientId,
//         message->crc,
//         message->dataSize);

//     return;
// }

// void readData(int socket, Message* message){

//     message->data = malloc(sizeof(uint8_t) * message->dataSize);
//     if(message->data == NULL){
//         logm(FATAL, "Error allocating memory for incoming data (%d bytes).",
//         sizeof(uint8_t) * message->dataSize);
//         exit(EXIT_FAILURE);
//     }

//     message->bytesReceived += readFromSocket(socket, message->data, message->dataSize);

//     // TODO: Data received logging.

//     return;
// }


 


// int readByte(int socket){

//     uint8_t newChar;
    
//     errno = 0;
//     int bytesRead = read(socket, &newChar, sizeof(uint8_t));

//     if(bytesRead == -1){
//         logm(ERROR, "Unable to read from socket");
//         return -1;
//     }

//     return (int)newChar;
// }

