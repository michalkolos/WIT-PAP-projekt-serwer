#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <cJSON.h>

#define TYPE_STRING "string"
#define TYPE_INT    "int"
#define TYPE_DOUBLE "double"
#define TYPE_BOOL   "bool"
#define TYPE_TABLE  "table"


cJSON* json_generateTable(int columnNo, char* name, ...);
void json_addRowToTable(cJSON* table, ...);
cJSON* json_serialToObject(Message* message);
char* json_objectToSerial(cJSON* json);
char* json_objectToSerialHumanReadable(cJSON* json);


#endif