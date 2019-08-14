#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "connection.h"
#include "messagehandlers.h"
#include "log.h"

uint32_t readIntFromSocket(int socket){
    uint32_t inInt;

    //TODO: read error handling
    read(socket, &inInt, sizeof(uint32_t));
    inInt = ntohl(inInt);

    return inInt;
}

void readMessageFromSocket(Message* message){

    message->messageId = readIntFromSocket(message->readSocket);
    message->clientId = readIntFromSocket(message->readSocket);
    message->messageLen = readIntFromSocket(message->readSocket);

    message->body = malloc(message->messageLen * sizeof(uint8_t));
    
    //TODO:read error handling
    read(message->readSocket, message->body, message->messageLen);

    // logm(DEBUG, "Received message:\n\t* messageID: %d\n\t* clientID: %d\n\t* message Length: %d\n\t%s\n",
    //     message->messageId,
    //     message->clientId,
    //     message->messageLen,
    //     message->body);

    runHandler(message);

    // printf("%s\n", message->body);

    return;
}

void messageFinish(Message* message){

    if(message->error == 0){
        logm(INFO, "Handled connection:\n \tprotocol: %d\n\t* client: %d\n\t* data received: %dB\n\t* data sent: %dB",
            message->messageId,
            message->clientId,
            message->bytesReceived,
            message->bytesSent);
    }

    // TODO: Implement messageFinnish.

    return;
}









