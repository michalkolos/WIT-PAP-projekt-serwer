#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#define BUFFER_LEN 1024




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



// "data":{[
//     {   "table1": ["col01":{"name", "type"},
//                 "col02":{"name", "type"}]
//         "table2": ["col01":{"name", "type"},
//                 "col02":{"name", "type"}]
//     ] }