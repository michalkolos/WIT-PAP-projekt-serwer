#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#include "connection.h"





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

    printf("\n");

    return totalBytesRead;
}

