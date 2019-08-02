#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>

#define BUFFER_LEN          1024


#define CONNECT_ERROR   1
#define READ_ERROR      2
#define WRITE_ERROR     3
#define READ_TYPE_ERROR 4
#define IN_HEADER_ERROR 5
#define IN_DATA_ERROR   6


typedef struct Message Message;
typedef struct MessageHandlers MessageHandlers;



struct Message{
    
    int         readSocket;
    int         error;

    uint8_t     protocolId;
    int         clientId;
    int         crc;            // TODO: Implement CRC for incoming messages.
    
    int         dataSize;
    uint8_t*    data;
    
    int         bytesReceived;
    int         bytesSent;
};

void messageInit(int socket, Message* message);
void readMessageFromSocket(Message* message);
void messageFinish(Message* message);

#endif