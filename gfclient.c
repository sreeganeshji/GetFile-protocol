
#include "gfclient-student.h"

// optional function for cleaup processing.
void gfc_cleanup(gfcrequest_t **gfr){
    
    printf("cleaning up\n");
    free((**gfr).scheme);
//     (**gfr).server = NULL;
    free((**gfr).server);
//     (**gfr).method = NULL;
    free((**gfr).method);
//     (**gfr).path = NULL;
    free((**gfr).path);
//     (**gfr).port = NULL;
    free((**gfr).port);
    
    if ((**gfr).serverStatusAllocated == 1) free((**gfr).serverStatus);
    if ((**gfr).serverSchemeAllocated == 1) free((**gfr).serverScheme);
        if ((**gfr).serverLengthAllocated == 1) free((**gfr).serverLength);
   
//     (**gfr).headerArguements = NULL;
//    free((**gfr).headerArguements);
//     (**gfr).headerFunc = NULL;
//    free((**gfr).headerFunc);
//     (**gfr).writearg = NULL;
//    free((**gfr).writearg);
//     (**gfr).writefunc = NULL;
//    free((**gfr).writefunc);
    close((**gfr).clientSocket);
    
    free(*gfr);
    
    (*gfr) = NULL;
    (gfr) = NULL;

//    free(gfr);
}

gfcrequest_t *gfc_create(){
    // dummy for now - need to fill this part in
    printf("magic of creation\n");
    gfcrequest_t* clientGfRequest = (gfcrequest_t*)malloc(sizeof(gfcrequest_t));
    clientGfRequest->scheme =malloc(strlen("GETFILE")+1);
strcpy(clientGfRequest->scheme , "GETFILE");
    clientGfRequest->method = malloc(strlen("GET")+1);
    strcpy(clientGfRequest->method , "GET");
    clientGfRequest->bytesReceived = 0;
    clientGfRequest->fileLength = 0;
    clientGfRequest->clientStatus = GF_OK;
    clientGfRequest->serverStatusAllocated = 0;
    clientGfRequest->serverLengthAllocated = 0;
    clientGfRequest->serverSchemeAllocated = 0;

//    return (gfcrequest_t *)NULL;
    return clientGfRequest;
}

size_t gfc_get_bytesreceived(gfcrequest_t **gfr){
    // not yet implemented
    printf("returning bytes received %d\n",(**gfr).bytesReceived);
    return (** gfr).bytesReceived;
//    return -1;
}

size_t gfc_get_filelen(gfcrequest_t **gfr){
    // not yet implemented
    printf("returning file length %d\n",(**gfr).fileLength);
    return(**gfr).fileLength;
//    return -1;
}

gfstatus_t gfc_get_status(gfcrequest_t **gfr){
    // not yet implemented
    printf("returning clientstatus %s\n",gfc_strstatus((**gfr).clientStatus));
    return(**gfr).clientStatus;
//    return -1;
}

void gfc_global_init(){
    printf("global_init\n");
}

void gfc_global_cleanup(){
    printf("global cleanup \n");
}

void initializeSessionParameters(gfcrequest_t* session)
{
    printf("sesion parameters \n");
    strcpy(session->scheme , "GETFILE");
    strcpy(session->method , "GET");
    
}



long findOffset(char* mainString, int mainStringSize)
{
//    printf("finding offset \n");
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

int setStatus(gfcrequest_t **gfr, char* status)
{
   if(strcmp("OK", status) == 0)
   {
       (**gfr).clientStatus = GF_OK;
       return 0;
   }
    else if(strcmp("FILE_NOT_FOUND", status) == 0)
    {
        (**gfr).clientStatus = GF_FILE_NOT_FOUND;
         return 1;
    }
    else if(strcmp("ERROR", status) == 0)
    {
        (**gfr).clientStatus = GF_ERROR;
         return 1;
    }
    else if(strcmp("INVALID", status) == 0)
    {
        (**gfr).clientStatus = GF_INVALID;
         return 1;
    }
  
    return -1;
}


char* findScheme(char* input,gfcrequest_t** gfr)
{
    (**gfr).serverScheme = malloc(20);
    (**gfr).serverSchemeAllocated = 1;
    int i = 0;
    for( ; input[i]!=' ' && input[i]!='\r' && i < 18; i++)
    {
        (**gfr).serverScheme[i] = input[i];
    }
    (**gfr).serverScheme[i] = '\0';
    return (**gfr).serverScheme;
}
char* findStatus(char* input, gfcrequest_t** gfr)
{
    (**gfr).serverStatus = malloc(20);
    (**gfr).serverStatusAllocated = 1;
    int i = 0;
    for( ; input[i]!=' ' && input[i]!='\r' && i < 18; i++)
    {
        (**gfr).serverStatus[i] = input[i];
    }
    (**gfr).serverStatus[i] = '\0';
    return (**gfr).serverStatus;
}
char* findLength(char* input,gfcrequest_t** gfr)
{
    (**gfr).serverLength = malloc(20);
    (**gfr).serverLengthAllocated = 1;
    
    int i = 0;
    for( ; input[i]!=' ' &&  input[i]!='\r' && i < 18; i++)
    {
        (**gfr).serverLength[i] = input[i];
    }
    (**gfr).serverLength[i] = '\0';
    return (**gfr).serverLength;
}

int gfc_perform(gfcrequest_t **gfr){
    printf("perform\n");
    // currently not implemented.  You fill this part in.
    
    /*

     2. create a socket with the given arguements
     3. connect to the server
     4. send the request containing the path, method, and scheme
     5. the server will send the status and if OK, the data
     6. while receiving the data, run the callback method passing hte data.
     7. close the connection.
     */
    
    struct addrinfo addressHint, *serverAddress;
    
    memset(&addressHint, 0, sizeof(addressHint));
    
    addressHint.ai_family = AF_UNSPEC;
    addressHint.ai_socktype = SOCK_STREAM;
    addressHint.ai_flags = AI_PASSIVE;
    
    int GetServerAddressStatus = getaddrinfo((**gfr).server, (**gfr).port, &addressHint, &serverAddress);
    if(GetServerAddressStatus != 0)
    {
        fprintf(stderr, "couldn't obtain the server address\n");
        exit(1);
    }
    printf("obtained address\n");
    int clientSocket = socket(serverAddress->ai_family, serverAddress->ai_socktype, 0);
    if(clientSocket < 0)
    {
        fprintf(stderr, "couldn't create clientSocket\n");
        exit(1);
    }
    printf("created socket\n");
    (**gfr).clientSocket = clientSocket;
    int socketConnectionStatus = connect(clientSocket, serverAddress->ai_addr, serverAddress->ai_addrlen);
    
    freeaddrinfo(serverAddress);
    
    if(socketConnectionStatus != 0)
    {
        fprintf(stderr,"couldn't connect the socket\n");
        exit(1);
    }
    printf("connected to socket\n");
    char* requestMessage = getFormattedClientHeader((**gfr).scheme, (**gfr).method, (**gfr).path, "\r\n\r\n");

    int sentLength = send(clientSocket, requestMessage, strlen(requestMessage), 0);
    
    
    if(sentLength < 0 )
    {
        fprintf(stderr,"coulnd't send the request\n");
        exit(1);
    }
    printf("sent request %s to server\n",requestMessage);
    free(requestMessage);
    /*
     buffer1, buffer2
     [  ][     ]   ]
     
     buffer1[65]
     buffer2 = buffer1 + already_received.
     if header is not parsed:
        find the offset from buffer1.
        if couldn't find:
            if receivedLength == 0:
                INVALID!
            else:
                already_received = receivedLength
            continue
            
        if could find:
            set offset
            set the dataPtr
            already_received = 0;
            headerParsed = True
     
     
     */
    char recvStatusBuffer[4096];
    memset(recvStatusBuffer,0,sizeof(recvStatusBuffer));
    _Bool parsedServerStatus = 0;
    char * dataPtr;
    int currentDataLength = 0;
    long offset = 0;
    long headerOffset = 0;
    
    printf("created socket and ready for receivingdata \n");
    
    while(1){
       size_t receivedLength = recv(clientSocket, recvStatusBuffer+headerOffset, sizeof(recvStatusBuffer)-headerOffset, 0);
        
    if(receivedLength < 0)
    {
        fprintf(stderr,"couldn't receive data from the server\n");
        return -1;
    }
        
        currentDataLength = receivedLength;
        printf("receivedLength %zu\n",receivedLength);
        if(!parsedServerStatus)
        {if(receivedLength == 0)
            {//didn't receive the full header but the connection is closed
            setStatus(gfr, "INVALID");
                printf("didn't receive full header\n");
            return -1;
            }
            offset = findOffset(recvStatusBuffer,receivedLength+headerOffset);
                       if(offset < 0)
                                 {
                                         headerOffset += receivedLength;
                                         continue;
                                 }
                       else{
            //extract the parsed information
            parsedServerStatus = 1;
                           printf("received %s\n",recvStatusBuffer);
                           printf("found offset %ld\n",offset);
            char* scheme = findScheme(recvStatusBuffer,gfr);
                        
            if(!scheme || strcmp(scheme,"GETFILE") != 0)
            {
                setStatus((gfr), "INVALID");
                
                return -1;
            }
                           
                           char* status = findStatus(recvStatusBuffer+strlen(scheme)+1,gfr);
                           printf("parsed status %s\n",status);
                           int setStatusResult = setStatus(gfr, status);
            if(setStatusResult <0)
                           {
                return -1;
            }
                           else if(setStatusResult >0)
                           {
                               return 0;
                           }
                           char* length = findLength(recvStatusBuffer+strlen(scheme)+strlen(status)+2,gfr);
            (**gfr).fileLength = atoi(length);

                           
            dataPtr = recvStatusBuffer+offset;
            
            
            currentDataLength = currentDataLength - offset + headerOffset;
                       }
        }
        else dataPtr = recvStatusBuffer;
        
        
        (**gfr).bytesReceived += currentDataLength;
//        printf("received %d of %d\n",(**gfr).bytesReceived,(**gfr).fileLength);
        (**gfr).writefunc(dataPtr,currentDataLength,(**gfr).writearg);
         
             
         
             if((**gfr).fileLength == (**gfr).bytesReceived){
                 printf("received all data, %d in total\n",(**gfr).bytesReceived);
            return 0;
             }
             else if(receivedLength == 0){
                 //didn't receive all the message
                 printf("didn't receive all message, received %d of %d\n",(**gfr).bytesReceived,(**gfr).fileLength);
                 return -1;
             }
         
         memset(recvStatusBuffer,0,sizeof(recvStatusBuffer));
    }
    
    return 0;
}

char* getFormattedClientHeader(char* scheme, char* method, char* path, char* delimiter){
    printf("get formatted client header\n");
    char* resStr;
    unsigned long totalLength = strlen(scheme)+strlen(method)+strlen(delimiter)+strlen(path)+3;
    resStr = (char*) malloc(sizeof(char)*(totalLength+1) );
    strcpy(resStr, scheme);
    strcat(resStr, " ");
    strcat(resStr, method);
    strcat(resStr, " ");
    strcat(resStr, path);
    strcat(resStr, " ");
    strcat(resStr, delimiter);
    return resStr;
}

void gfc_set_headerarg(gfcrequest_t **gfr, void *headerarg){
    printf("set header arg\n");
    (**gfr).headerArguements = headerarg;
}

void gfc_set_headerfunc(gfcrequest_t **gfr, void (*headerfunc)(void*, size_t, void *)){
    printf("set header func\n");
    (**gfr).headerFunc = headerfunc;
}

void gfc_set_path(gfcrequest_t **gfr, const char* path){
    printf("set path\n");
    (**gfr).path = malloc(strlen(path)+1);
    strcpy((**gfr).path , path);
}

void gfc_set_port(gfcrequest_t **gfr, unsigned short port){
    printf("set port\n");
    (**gfr).port = (char*)malloc(sizeof(char)*20);
    snprintf((**gfr).port, 20, "%d",port);

}

void gfc_set_server(gfcrequest_t **gfr, const char* server){
    printf("set server\n");
  (**gfr).server = (char*)malloc(sizeof(char)*20);
    snprintf((**gfr).server, 20, "%s",server);

}

void gfc_set_writearg(gfcrequest_t **gfr, void *writearg){
    printf("set writearg\n");
    (**gfr).writearg = writearg;
}

void gfc_set_writefunc(gfcrequest_t **gfr, void (*writefunc)(void*, size_t, void *)){
    printf("set write func\n");
    (**gfr).writefunc = writefunc;
}

const char* gfc_strstatus(gfstatus_t status){
    printf("get string status ");
    const char *strstatus = NULL;

    switch (status)
    {
        default: {
            strstatus = "UNKNOWN";
        }
        break;

        case GF_INVALID: {
            strstatus = "INVALID";
        }
        break;

        case GF_FILE_NOT_FOUND: {
            strstatus = "FILE_NOT_FOUND";
        }
        break;

        case GF_ERROR: {
            strstatus = "ERROR";
        }
        break;

        case GF_OK: {
            strstatus = "OK";
        }
        break;
        
    }
    
    printf("%s \n",strstatus);

    return strstatus;
}

