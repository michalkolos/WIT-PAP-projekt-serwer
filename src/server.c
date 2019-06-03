#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h> 	// API and functionality for creating sockets
#include <netinet/in.h> 	// Structures storing address information
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "log.h"
#include "connectionqueue.h"

int startServer(int serverIP, int serverPort, struct sockaddr_in* serverAddress){

    ///Creating server socket
    errno = 0;
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
	{
        logm(FATAL, "Unable to create server socket | %s", strerror(errno));
	}
    else
    {
        logm(DEBUG, "Created server socket: %d", serverSocket);
    }


    ///Creating server address struct

        memset(serverAddress, 0, sizeof(*serverAddress));

	serverAddress->sin_family        = AF_INET;
    serverAddress->sin_port          = htons(serverPort);
    serverAddress->sin_addr.s_addr   = serverIP;

    char addressString[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddress->sin_addr.s_addr), addressString, INET_ADDRSTRLEN);
    logm(DEBUG, "Created address struct for address: %s:%d.",addressString, serverPort);



    ///Binding server address to the socket
    errno = 0;
	int bindStatus = bind(serverSocket,
        (struct sockaddr*) serverAddress,
        sizeof(*serverAddress)
    );

    if(bindStatus == -1)
	{
        logm(FATAL, "Unable to bind address to server socket | %s", strerror(errno));
	}
    else
    {
        logm(DEBUG, "Address bound to server socket.");
    }

    ///Creating connection queue on the server socket
    errno = 0;
	int listenStatus = listen(serverSocket, 5);

    if(listenStatus == -1)
	{
        logm(FATAL, "Unable to listen on server socket | %s", strerror(errno));

	}
    else
    {
        logm(INFO, "Server listening on socket: %d, bound to address: %s:%d",
            serverSocket,
            addressString,
            serverPort);
    }

    return serverSocket;
}




void serverAcceptConnection(int serverSocket, ConnectionQueue* connectionQueue){
    int clientSocket;
    
    struct sockaddr_in clientAddress;
    memset(&clientAddress, 0, sizeof(clientAddress));

    socklen_t sizeOfClientAddress = sizeof(clientAddress);

    errno = 0;
    clientSocket =  accept(serverSocket, (struct sockaddr *) &clientAddress, &sizeOfClientAddress);

    char addressString[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddress.sin_addr.s_addr), addressString, INET_ADDRSTRLEN);

    if(clientSocket == -1){
        logm(ERROR, "Unable to connect with client: %s:%d | %s",
            addressString,
            ntohs(clientAddress.sin_port),
            strerror(errno));
    }else{
        logm(INFO, "Accepted connection with client: %s:%d ",
            addressString,
            ntohs(clientAddress.sin_port));
    }

    connectionQueuePush(connectionQueue, clientSocket);

    return;
}

 