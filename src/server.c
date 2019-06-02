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

int startServer(int serverIP, int serverPort, struct sockaddr_in* serverAddress, LogQueue* logq){

    ///Creating server socket
    errno = 0;
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
	{
        logm(logq, FATAL, "Unable to create server socket | %s", strerror(errno));
	}
    else
    {
        logm(logq, DEBUG, "Created server socket: %d", serverSocket);
    }


    ///Creating server address struct

        memset(serverAddress, 0, sizeof(*serverAddress));

	serverAddress->sin_family        = AF_INET;
    serverAddress->sin_port          = htons(serverPort);
    serverAddress->sin_addr.s_addr   = serverIP;

    char addressString[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddress->sin_addr.s_addr), addressString, INET_ADDRSTRLEN);
    logm(logq, DEBUG, "Created address struct for address: %s:%d.",addressString, serverPort);



    ///Binding server address to the socket
    errno = 0;
	int bindStatus = bind(serverSocket,
        (struct sockaddr*) serverAddress,
        sizeof(*serverAddress)
    );

    if(bindStatus == -1)
	{
        logm(logq, FATAL, "Unable to bind address to server socket | %s", strerror(errno));
	}
    else
    {
        logm(logq, DEBUG, "Address bound to server socket.");
    }

    ///Creating connection queue on the server socket
    errno = 0;
	int listenStatus = listen(serverSocket, 5);

    if(listenStatus == -1)
	{
        logm(logq, FATAL, "Unable to listen on server socket | %s", strerror(errno));

	}
    else
    {
        logm(logq, INFO, "Server listening on socket: %d, bound to address: %s:%d",
            serverSocket,
            addressString,
            serverPort);
    }

    return serverSocket;
}