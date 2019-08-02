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




void messageInit(int socket, Message* message){

    message->readSocket = socket;
    message->error = 0;
    message->bytesReceived = 0;
    message->bytesSent = 0;
    message->dataSize = 0;
    message->crc = 0;
    message->protocolId = 0;

    logm(DEBUG, "Created new message struct for incoming message [socket: %d]",
        message->readSocket);

    return;
}

void readMessageFromSocket(Message* message){

    if(message == NULL){

        logm(ERROR, "Message struct not initialized.");
        // TODO: Handle uninitialized message struct error.
    }
    
    errno = 0;
    int bytesRead = read(message->readSocket, &message->protocolId, sizeof(uint8_t));

    if(bytesRead == -1){
        logm(ERROR, "Unable to read message type: %s", strerror(errno));
        message->error = READ_TYPE_ERROR;
        return;
    }

    logm(DEBUG, "Determined that incoming message is labeled as: %d", 
        (int)message->protocolId);

    runHandler(message);

    return;
}

void messageFinish(Message* message){

    if(message->error == 0){
        logm(INFO, "Handled connection:\n \tprotocol: %d\n\t* client: %d\n\t* data received: %dB\n\t* data sent: %dB",
            message->protocolId,
            message->clientId,
            message->bytesReceived,
            message->bytesSent);
    }

    // TODO: Implement messageFinnish.

    return;
}









