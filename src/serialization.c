#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "cJSON.h"
#include "log.h"
#include "connection.h"
#include "serialization.h"


cJSON* json_serialToObject(Message* message){
    cJSON* json = cJSON_Parse(message->body);
    
    if (json == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        
        if (error_ptr != NULL){
            logm(ERROR, "Error parsing JSON before: %s", error_ptr);
        }
        
        return NULL;
    }

    return json;
}

char* json_objectToSerial(cJSON* json){
    char* string = NULL;
    
    string = cJSON_PrintUnformatted(json);
    if (string == NULL) {
        logm(ERROR, "Failed to serialize JSON object");

        return NULL;
    }

    return string;
}

char* json_objectToSerialHumanReadable(cJSON* json){
    char* string = NULL;
    
    string = cJSON_Print(json);
    if (string == NULL) {
        logm(ERROR, "Failed to serialize JSON object");

        return NULL;
    }

    return string;
}




void DefaultJsonBuildError(){
    logm(ERROR, "Error building JSON");

    return;
}

cJSON* json_generateTable(int columnNo, char* name, ...){

    cJSON* table = cJSON_CreateObject();

    if (cJSON_AddStringToObject(table, "name", name) == NULL){
        DefaultJsonBuildError();
        return NULL;
    }

    cJSON* header = NULL;
    header = cJSON_AddArrayToObject(table, "header");
    if (header == NULL){
        DefaultJsonBuildError();
        return NULL;
    }

    va_list arg;
    va_start(arg, name);

    for(int i = 0; i < columnNo; i++){   

        cJSON* column = cJSON_CreateObject();
        if (cJSON_AddStringToObject(column, "name", va_arg(arg, char*)) == NULL){
            DefaultJsonBuildError();
        }

        if (cJSON_AddStringToObject(column, "type", va_arg(arg, char*)) == NULL){
            DefaultJsonBuildError();
        }

        cJSON_AddItemToArray(header, column); 
    }
    
    va_end(arg);

    cJSON* data = NULL;
    data = cJSON_AddArrayToObject(table, "data");
    if (data == NULL){
        DefaultJsonBuildError();
        return NULL;
    }

    return table;
}

void json_addRowToTable(cJSON* table, ...){
    cJSON* header = cJSON_GetObjectItem(table, "header");
    int columnNo = cJSON_GetArraySize(header);
    cJSON* data = cJSON_GetObjectItem(table, "data");

    cJSON* newRow = NULL;
    newRow = cJSON_CreateArray();
    if (newRow == NULL){
        DefaultJsonBuildError();
        return;
    }

    cJSON* type;
    char* typeString;
    cJSON* newVal;

    va_list arg;
    
    va_start(arg, table);
    
    for(int i = 0; i < columnNo; i++){
        type = cJSON_GetObjectItem(cJSON_GetArrayItem(header, i), "type");
        typeString = cJSON_GetStringValue(type);
        if(!strcmp(typeString, TYPE_INT)){
            newVal = cJSON_CreateNumber(va_arg(arg, int));
        }else if(!strcmp(typeString, TYPE_DOUBLE)){
            newVal = cJSON_CreateNumber(va_arg(arg, double));
        }else if (!strcmp(typeString, TYPE_STRING)){
            char* string = va_arg(arg, char*);
            newVal = cJSON_CreateString(string);
        }else if (!strcmp(typeString, TYPE_BOOL)){
            newVal = cJSON_CreateBool(va_arg(arg, int));
        }else if (!strcmp(typeString, TYPE_TABLE)){
            newVal = va_arg(arg, cJSON*);
        }
        
        cJSON_AddItemToArray(newRow, newVal);
    }

    va_end(arg);
    cJSON_AddItemToArray(data, newRow);
    return;
}

int deleteJson(cJSON* json){
    cJSON_Delete(json);
}

void messageParseJson(Message* message){
    //TODO: Implement messageParseTable();
}