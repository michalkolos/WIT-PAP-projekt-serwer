#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

int startServer(int serverIP, int serverPort, struct sockaddr_in* serverAddress);
int acceptConnection(int socket, struct sockaddr_in* clientAddress);

#endif