
#include "gfserver-student.h"

/*
 * Modify this file to implement the interface specified in
 * gfserver.h.
 */

void gfs_abort(gfcontext_t **ctx){
    printf("abort mission\n");
    close((**ctx).clientSocket);
}

gfserver_t* gfserver_create(){
    printf("expanding my creativity\n");
    gfserver_t* serverInstance;
    serverInstance = (gfserver_t*)malloc(sizeof(gfserver_t));
    return serverInstance;
//    return (gfserver_t *)NULL;
}

ssize_t gfs_send(gfcontext_t **ctx, const void *data, size_t len){
//    printf("sending thought the wires\n");
    
//    int sentSize =send((**ctx).clientSocket, data, 60, 0);
//
//    close((**ctx).clientSocket);
//    return sentSize;
    
    return send((**ctx).clientSocket, data, len, 0);
    
}

ssize_t gfs_sendheader_error(gfcontext_t **ctx, gfstatus_t status){
    
    printf("sending the error");
    char* endingStr = "\r\n\r\n";
    char* scheme = "GETFILE";
    char statusChar [20];//maximum length of the status
    memset(statusChar, 0, sizeof(statusChar));
    switch (status) {
            
        case GF_ERROR:
            //error on hte server side
            strcat(statusChar, "ERROR");
            break;
        case GF_INVALID:
        //header from the client is invalid
            strcat(statusChar, "INVALID");
        break;
        default:
            break;
    }
    
    printf("%s\n",statusChar);
    char resMessage[50];
    memset(resMessage,0,sizeof(resMessage));
    strcat(resMessage, scheme);
    strcat(resMessage, " ");
    strcat(resMessage, statusChar);
    strcat(resMessage, " ");
    strcat(resMessage, endingStr);
    strcat(resMessage, " ");
    
    return send((**ctx).clientSocket, resMessage, strlen(resMessage), 0);
}



ssize_t gfs_sendheader(gfcontext_t **ctx, gfstatus_t status, size_t file_len){
    
    char* endingStr = "\r\n\r\n";
    char* scheme = "GETFILE";
    char statusChar [20];//maximum length of the status
    char lengthChar[30];
    memset(statusChar, 0, sizeof(statusChar));
    memset(lengthChar, 0, sizeof(lengthChar));
    int isValid = 0;
    
//    strcpy(statusChar,scheme);
//    strcat(statusChar,"somestuff");
////    strcat(statusChar," ");
////    strcat(statusChar, "ERROR");
////    strcat(statusChar, " ");
////    strcat(statusChar, endingStr);
//    send((**ctx).clientSocket,statusChar,strlen(statusChar),0);
//    close((**ctx).clientSocket);
    
    switch (status) {
            
        case GF_OK:
            //send the scheme, status and length
            strcpy(statusChar, "OK");
            
            snprintf(lengthChar, 10, "%zu",file_len);
            isValid = 1;
            break;
        case GF_FILE_NOT_FOUND:
            strcpy(statusChar, "FILE_NOT_FOUND");
            break;
            
        case GF_ERROR:
            strcpy(statusChar, "ERROR");
            break;

        default:
            break;
    }

    printf("sending status %s\n",statusChar);
    char resMessage[50];
    memset(resMessage,0,sizeof(resMessage));
    strcpy(resMessage, scheme);
    strcat(resMessage, " ");
    strcat(resMessage, statusChar);
    strcat(resMessage, " ");
    if(isValid == 1)
    {
        strcat(resMessage, lengthChar);
        strcat(resMessage, " ");
    }
    strcat(resMessage, endingStr);
//    strcat(resMessage, " ");
    
    return send((**ctx).clientSocket, resMessage, strlen(resMessage), 0);
}

long findOffset(char* mainString, int mainStringSize)
{
    printf("finding offset \n");
    char * tailString = "\r\n\r\n";
    int tN = 4;
    int N = mainStringSize;
    int tailInd = 0;

    for (int mainInd = 0; mainInd <N; mainInd++)
    {
        if(mainString[mainInd] == tailString[tailInd])
        {
            tailInd ++ ;
            if(tailInd == tN)
            {
                return(mainInd+1);
            }
        }
        else{
            tailInd = 0;
        }
    }
    
    return -1; //header invalid status
}

char* findScheme(char* input,requestPacket ** gfr)
{
    (**gfr).scheme = malloc(20);
    (**gfr).schemeAlloc = 1;
    int i = 0;
    for( ; input[i]!=' ' && input[i]!='\r' && i < 18; i++)
    {
        (**gfr).scheme[i] = input[i];
    }
    (**gfr).scheme[i] = '\0';
    printf("found scheme %s\n",(**gfr).scheme);
    return (**gfr).scheme;
    
}
char* findMethod(char* input, requestPacket** gfr)
{
    (**gfr).method = malloc(20);
    (**gfr).methodAlloc = 1;
    int i = 0;
    for( ; input[i]!=' ' && input[i]!='\r' && i < 18; i++)
    {
        (**gfr).method[i] = input[i];
    }
    (**gfr).method[i] = '\0';
    printf("found method %s\n",(**gfr).method);
    return (**gfr).method;
}
char* findPath(char* input,requestPacket** gfr)
{
    (**gfr).path = malloc(200);
    (**gfr).pathAlloc = 1;
    
    int i = 0;
    for( ; input[i]!=' ' &&  input[i]!='\r' && i < 198; i++)
    {
        (**gfr).path[i] = input[i];
    }
    (**gfr).path[i] = '\0';
    printf("found path %s\n",(**gfr).path);
    return (**gfr).path;
}


void gfserver_serve(gfserver_t **gfs){

    /*
     * create a socke and receive the request
     * validate the request
     *
     */
    //initialization
   //gfstatus_t contextStatus = GF_OK;

    

    gfcontext_t *currentContext = malloc(sizeof(gfcontext_t));
    
    struct addrinfo addressHint, *serverAddress;
    memset(&addressHint, 0, sizeof(addressHint));

    addressHint.ai_family = AF_UNSPEC;
    addressHint.ai_socktype = SOCK_STREAM;
    addressHint.ai_flags = AI_PASSIVE;

   int serverGetAddressStatus =  getaddrinfo(NULL, (**gfs).portNumberChar, &addressHint, &serverAddress);
    if (serverGetAddressStatus != 0 )
    {
        fprintf(stderr,"couldn't get server address\n");
        exit(1);
    }

    int serverSocket = socket(serverAddress->ai_family, serverAddress->ai_socktype, 0);
    if(serverSocket < 0)
    {
        fprintf(stderr,"couldn't make socket\n");
        exit(1);
    }
    
    int serverSocketOption = 1;
    int serverSocketOptionStatus = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &serverSocketOption, sizeof(int));

    if(serverSocketOptionStatus != 0)
    {
    fprintf(stderr,"couldn't change the options\n");
    exit(1);
    }

    int bindingStatus = bind(serverSocket, serverAddress->ai_addr, serverAddress->ai_addrlen);
    if(bindingStatus !=0 )
    {
        fprintf(stderr,"couldn't bind socket\n");
        exit(1);
    }
    
    int listeningStatus = listen(serverSocket, (**gfs).maxPending);
    if(listeningStatus != 0 )
    {
        fprintf(stderr,"listening failed\n");
        exit(1);
    }
    
    printf("ready to send \n");
    
    while(1){
        int newClientSocket = accept(serverSocket, serverAddress->ai_addr, &serverAddress->ai_addrlen);
        
        printf("accepted a new socket connection %d\n",newClientSocket);
        if(newClientSocket <0 )
        {
            //ERROR status
            printf("couldn't create socket\n");
            fprintf(stderr,"couldn't create client socket\n");
            gfs_sendheader_error((&currentContext), GF_ERROR);
            gfs_abort(&currentContext);
            continue;
        }
        currentContext->clientSocket = newClientSocket;
        
//        gfs_sendheader_error((&currentContext), GF_ERROR);
//        close(newClientSocket);
//        break;
requestPacket *thisRequest = malloc(sizeof(requestPacket));
        thisRequest->method = "";
        thisRequest->scheme = "";
        thisRequest->path = "";
        
        char buffer [100];
    memset(buffer,0,sizeof(buffer));
        /*
         receive the command form the client and send  the requested file
         */

        size_t receivedBytes;
        int haveFoundTheEnd = 0;
        int totalReceivedSize = 0;
        int offset = 0;
        int cond1 = 1;
        int cond2 = 1;
        _Bool cond3 = 1;
        while(1){
            //receive client header
        receivedBytes = recv(newClientSocket, buffer+offset, sizeof(buffer)-offset, 0);
            printf("received %ld\n%s\n",receivedBytes,buffer);
       
    if(receivedBytes < 0)
    {
        fprintf(stderr,"couldn't receive from the client");
        gfs_sendheader_error((&currentContext), GF_ERROR);
        gfs_abort(&currentContext);
        break;
    }
            
            totalReceivedSize += receivedBytes;
            
            if(!haveFoundTheEnd)
            {
                if(receivedBytes == 0)
                {
                    //didn't complete the request
                    gfs_abort(&currentContext);
                    break;
                }
                //find the ending element, if not found, wait for more data
                int endInd = findOffset(buffer, totalReceivedSize);
                if(endInd<0)
                {
                    offset += receivedBytes;
                    printf("waiting for more data\n");
                    continue;
                }
                haveFoundTheEnd = 1;
                
            }
            
        //parse the clients request
            char* scheme = findScheme(buffer, &thisRequest);
        char* method = findMethod(buffer+strlen(scheme)+1,&thisRequest);
        char* path = findPath(buffer+strlen(scheme)+strlen(method)+2, &thisRequest);
       
        cond1 = strcmp(scheme,"GETFILE");
        cond2 = strcmp(method,"GET");
        cond3 = (path == NULL) || path[0]!='/';
            break;
        }
    
        if(cond1 || cond2 || cond3)
        {
            //contextStatus = GF_INVALID;
            printf("condition not staisfied, abortind \n");
            gfs_sendheader_error((&currentContext), GF_INVALID);
        
        }
        else{
            printf("calling the handler\n");
            gfh_error_t handlerResult = (**gfs).handler((&currentContext),thisRequest->path,(**gfs).handlerArg);
            if(handlerResult<0)
            {
                //contextStatus = GF_FILE_NOT_FOUND;
            }
        }
        free(thisRequest->method);
                free(thisRequest->path);
                free(thisRequest->scheme);
        free(thisRequest);
        thisRequest = NULL;
        //close the socket
//        printf("closing the connection\n");
//        close(newClientSocket);
//        gfs_abort(&currentContext);
    }
}

void gfserver_set_handlerarg(gfserver_t **gfs, void* arg){
    //(**gfs).handlerArg = malloc(sizeof(arg));
    (**gfs).handlerArg = arg;
}

void gfserver_set_handler(gfserver_t **gfs, gfh_error_t (*handler)(gfcontext_t **, const char *, void*)){
   // (**gfs).handler = malloc(sizeof(handler)+1);
    (**gfs).handler = handler;
}

void gfserver_set_maxpending(gfserver_t **gfs, int max_npending){
    (**gfs).maxPending = max_npending;
}

void gfserver_set_port(gfserver_t **gfs, unsigned short port){
    (**gfs).portNumberChar = (char*)malloc(sizeof(char)*20);
       snprintf((**gfs).portNumberChar, 20, "%d",port);
}


