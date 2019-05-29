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
	}
    else
    {
        // TODO: Reporting created socket.
    }


    ///Creating server address struct

        memset(serverAddress, 0, sizeof(*serverAddress));

	serverAddress->sin_family        = AF_INET;
    serverAddress->sin_port          = serverPort;
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
	}
    else
    {
        // TODO: Address binding reporting
    }

    ///Creating connection queue on the server socket
    errno = 0;
	int listenStatus = listen(serverSocket, 5);

    if(listenStatus == -1)
	{
        // TODO: Connection queue creation error reporting
	}
    else
    {
        // TODO: Connection queue creation reporting
    }

    return serverSocket;
}