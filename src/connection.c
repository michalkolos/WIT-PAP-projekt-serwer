#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "connection.h"
#include "log.h"


typedef struct Message Message;

struct Message{
    uint32_t    protocolId;
    uint32_t    clientId;
    uint32_t    crc;
    
    uint32_t    dataSize;
    uint8_t*    data;
    
    uint32_t    bytesReceived;
    uint32_t    bytesSent;
};


int readFromSocket(int socket, char* buffer, int size){

    int totalBytesRead = 0;
    int bytesRead = 0;
    
    errno = 0;
    while ( (bytesRead = read(socket, buffer, size - 1)) > 0)
    {
        totalBytesRead += bytesRead;
        buffer[bytesRead] = 0;
        printf("%s", buffer);
    }

    // TODO: Read from socket error handling.

    printf("\n");

    return totalBytesRead;
}

uint32_t parseInt(char* buffer){
    uint32_t value = (uint32_t)buffer[0];
    value = ntohs(value);

    return value;
}

void readHeader(int socket, Message* message){

    char buffer[BUFFER_LEN];
    message->bytesReceived += readFromSocket(socket, buffer, BUFFER_LEN);

    int index = 0;

    message->protocolId = parseInt(buffer + index);
    index += sizeof(uint32_t);

    message->clientId = parseInt(buffer + index);
    index += sizeof(uint32_t);

    message->crc = parseInt(buffer + index);
    index += sizeof(uint32_t);

    message->dataSize = parseInt(buffer + index);
    index += sizeof(uint32_t);

    logm(DEBUG, "Received header: protocol: %d//client: %d//crc: %d//data size: %d",
        message->protocolId,
        message->clientId,
        message->crc,
        message->dataSize);

    return;
}

void readData(int socket, Message* message){

    message->data = malloc(sizeof(uint8_t) * message->dataSize);
    if(message->data == NULL){
        logm(FATAL, "Error allocating memory for incomming data (%d bytes).",
        sizeof(uint8_t) * message->dataSize);
        exit(EXIT_FAILURE);
    }

    message->bytesReceived += readFromSocket(socket, message->data, message->dataSize);

    // TODO: Data received logging.

    return;
}

 
void connectionHandler(int socket){

    Message message;

    readHeader(socket, &message);

    return;
}