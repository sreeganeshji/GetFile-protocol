/*
 *  This file is for use by students to define anything they wish.  It is used by the gf server implementation
 */
#ifndef __GF_SERVER_STUDENT_H__
#define __GF_SERVER_STUDENT_H__

#include "gf-student.h"
#include "gfserver.h"

struct gfserver_t{
    char* portNumberChar;
    int maxPending;
    gfh_error_t (*handler)(gfcontext_t**,const char *,void*);
    void* handlerArg;
  
};

typedef struct requestPacket {
    char* scheme;
    char* method;
    char* path;
    int schemeAlloc;
    int methodAlloc;
    int pathAlloc;
} requestPacket;

struct gfcontext_t{
    int clientSocket;
    
};

#endif // __GF_SERVER_STUDENT_H__
