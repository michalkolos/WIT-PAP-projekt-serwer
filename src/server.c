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


int startServer(int serverIP, int serverPort, struct sockaddr_in* serverAddress){

    ///Creating server socket
    errno = 0;
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
	{
        // TODO: Creating socket error handling
        printf("Error socket\n");
	}
    else
    {
        // TODO: Reporting created socket.
        printf("Socket ok\n");
    }


    ///Creating server address struct

        memset(serverAddress, 0, sizeof(*serverAddress));

	serverAddress->sin_family        = AF_INET;
    serverAddress->sin_port          = htons(serverPort);
    serverAddress->sin_addr.s_addr   = serverIP;

    // TODO: Report creation of address struct



    ///Binding server address to the socket
    errno = 0;
	int bindStatus = bind(serverSocket,
        (struct sockaddr*) serverAddress,
        sizeof(*serverAddress)
    );

    if(bindStatus == -1)
	{
        // TODO: Binding address error handling
        printf("Error bind\n");
	}
    else
    {
        // TODO: Address binding reporting
        printf("Bind ok\n");
    }

    ///Creating connection queue on the server socket
    errno = 0;
	int listenStatus = listen(serverSocket, 5);

    if(listenStatus == -1)
	{
        // TODO: Connection queue creation error reporting
        printf("Error listen\n");
	}
    else
    {
        // TODO: Connection queue creation reporting
        printf("Listen ok\n");
    }

    return serverSocket;
}


int acceptConnection(int socket, struct sockaddr_in* clientAddress){

    int clientSocket;
    socklen_t sizeOfClientAddress = sizeof(clientAddress);
    
    errno = 0;
    clientSocket =  accept(socket, (struct sockaddr *) clientAddress, &sizeOfClientAddress);
 
    if (clientSocket == -1){
        // TODO: Handle server accept errors.
    }

    return clientSocket;
}