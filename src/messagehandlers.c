#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <arpa/inet.h>

#include "messagehandlers.h"
#include "connection.h"
#include "log.h"

#include "cJSON.h"


MessageHandlers* messageHandlers;



void messageHandlersInit(){

    messageHandlers = malloc(sizeof(MessageHandlers));
    if(messageHandlers == NULL){
        logm(FATAL, "Unable to allocate memory for a new Connection struct.");
        exit(EXIT_FAILURE);
    } 

    messageHandlers->size = MESSAGE_HANDLER_NO;
    messageHandlers->free = MESSAGE_HANDLER_NO;

    addHandler(errorMessageHandler, 0, "Error");

    for(int i = 1; i < MESSAGE_HANDLER_NO - 1; i++){
        messageHandlers->pointers[i] = unknownMessageHandler;
        messageHandlers->handlerState[i] = 0;
    } 

    addHandler(testMessageHandler, 1, "test");

    char logMessage[LOG_MESSAGE_LEN];
    char concatMessage[LOG_MESSAGE_LEN];
    snprintf(logMessage, LOG_MESSAGE_LEN, 
        "Initialised message handler table. Server accepts following messages:\n");

    for (int i = 1; i < MESSAGE_HANDLER_NO; i++){
        if(messageHandlers->handlerState[i] == 1){
            snprintf(concatMessage, LOG_MESSAGE_LEN, 
                "\t* code: %3d - %s\n",
                i, 
                &messageHandlers->handlerName[i][0]);

            strcat(logMessage, concatMessage);
        }
        
    }
    snprintf(concatMessage, LOG_MESSAGE_LEN, 
        "Free spots for additional handlers: %d",
        messageHandlers->free);

        strcat(logMessage, concatMessage);
    
    logm(INFO, logMessage);

    return;
}


void runHandler(Message* message){

    return (messageHandlers->pointers[message->messageId])(message);
}


void addHandler(void (*handlerPointer) (Message* message), int position, char* name){

    if(position >= MESSAGE_HANDLER_NO || messageHandlers->handlerState[position] != 0){
        logm(ERROR, "Unable to add new message handler.");
        return;
    }
    
    snprintf(&messageHandlers->handlerName[position][0], LOG_MESSAGE_LEN, name);

    messageHandlers->pointers[position] = handlerPointer;
    messageHandlers->handlerState[position] = 1;
    messageHandlers->free--;

    return;
}



//  /$$   /$$                           /$$ /$$                              
// | $$  | $$                          | $$| $$                              
// | $$  | $$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$$
// | $$$$$$$$ |____  $$| $$__  $$ /$$__  $$| $$ /$$__  $$ /$$__  $$ /$$_____/
// | $$__  $$  /$$$$$$$| $$  \ $$| $$  | $$| $$| $$$$$$$$| $$  \__/|  $$$$$$ 
// | $$  | $$ /$$__  $$| $$  | $$| $$  | $$| $$| $$_____/| $$       \____  $$
// | $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$|  $$$$$$$| $$       /$$$$$$$/
// |__/  |__/ \_______/|__/  |__/ \_______/|__/ \_______/|__/      |_______/ 
                                                                          
                                                                          
                                                                          


void unknownMessageHandler(Message* message){

    logm(ERROR, "Unknown message format");
    // TODO: Implement unknownMessageHandler.
    return;
}

void errorMessageHandler(Message* message){

    logm(ERROR, "Received bad message.");
    // TODO: Implement errorMessageHandler.
    return;
}

void testMessageHandler(Message* message){
 
    int messageLen = messageTotalLen(message) + 100;
 
    char messageString [messageLen];
    messageToString(message, messageString);

    logm(DEBUG, "%s \n", messageString);
    
    return;
}




//    _     _      _     _      _     _      _     _      _     _      _     _      _     _      _     _   
//   (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)    (c).-.(c)  
//    / ._. \      / ._. \      / ._. \      / ._. \      / ._. \      / ._. \      / ._. \      / ._. \   
//  __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__  __\( Y )/__ 
// (_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)(_.-/'-'\-._)
//    || H ||      || A ||      || N ||      || D ||      || L ||      || E ||      || R ||      || S ||   
//  _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._  _.' `-' '._ 
// (.-./`-'\.-.)(.-./`-'\.-.)(.-./`-'\.-.)(.-./`-'\.-.)(.-./`-'\.-.)(.-./`-'\.-.)(.-./`-`\.-.)(.-./`-`\.-.)
//  `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-'  `-'     `-' 


int messageTotalLen(Message* message){
    int len = message->messageLen   + sizeof(message->messageId) 
                                    + sizeof(message->clientId)
                                    + sizeof(message->bytesSent);
    return len;
}

void messageToString(Message* message, char* buffer){

    sprintf(buffer, "Received message:\n\t* messageID: %d\n\t* clientID: %d\n\t* message Length: %d\n\t%s\n",
        message->messageId,
        message->clientId,
        message->messageLen,
        message->body);

    // printf("%s\n", message->body);

        return;
}

void messageParseJson(Message* message){

}