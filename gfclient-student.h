/*
 *  This file is for use by students to define anything they wish.  It is used by the gf client implementation
 */
 #ifndef __GF_CLIENT_STUDENT_H__
 #define __GF_CLIENT_STUDENT_H__
 
 #include "gfclient.h"
 #include "gf-student.h"
 
struct gfcrequest_t{
    char* server;
    char* path;
    char* port;
    void(*writefunc)(void*, size_t, void*);
    void *writearg;
    int bytesReceived;
    int fileLength;
    gfstatus_t clientStatus;
    void(*headerFunc)(void*,size_t,void*);
    void *headerArguements;
    char* scheme;
    char* method;
    int clientSocket;
    char* serverScheme;
    char* serverStatus;
    char* serverLength;
    int serverSchemeAllocated;
    int serverStatusAllocated;
    int serverLengthAllocated;
};

char* getFormattedClientHeader(char* scheme, char* method, char* path, char* delimiter);

 #endif // __GF_CLIENT_STUDENT_H__
