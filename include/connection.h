#ifndef CONNECTION_H
#define CONNECTION_H

#define BUFFER_LEN 1024

int readFromSocket(int socket, char* buffer, int size);

#endif