# GetFile-protocol

GetFile is a GET protocol used to request files from a server connected via the IP address.

Protocol definition

General request:
```
<scheme> <method> <filename> \r\n\r\n
```
General response:
```
<scheme> <method> <status> <content> \r\n\r\n
```
Where,
```
scheme = GETFILE
method = GET
status = OK, FILE_NOT_FOUND, INVALID, ERROR

INVALID: In the case that the header received from the client was invalid.
ERROR: Server error.
```
# Design
This projects implements the get file protocol in which the client requests for a file using its path to the server and the server responds with the status of the operation and the file if successful. 

<image src="https://raw.githubusercontent.com/sreeganeshji/socket-echo-client-server/master/Illustrations/gf.png" height=150>

# Tradeoffs considered
The server needs to be able to parse the request from the client. The received request for the client is passed through a parsing function which extracts the relevant information or detects errors in the process. Similarly the client needs to parse the request received by the server. Both server and client needs to handle partial messages such that it needs to cumulate the message chunks till it has detected the tail string at which point it can extract the relevant information. 
# Flow of control
The server creates a handle variable to the server which holds all the relevant information pertaining to the connection, file, memory, handler function, etc. it initializes the connection and prepares to accept client request indefinitely. 
The client creates a handle variable for the request which is initialized with the requested parameters of the server address, path of the file, port and a write back function. The client sends a request using the socket. The server replies with the status of the data transfer and with data if successful. The client receives this data in chunks and passes it along to the write back function which can be defined to be independent of the protocol. Once all the data has been received, the client cleans all the memory that was allocated for its operation and exits. 
# Code testing
The gflibrary was tested to handle the different responses based on the data conditions including wrong path, scheme, method, and the proper functionality for large and small files, with small chunks of data and large chunks.  
# References
The socket connectivity part of the library was helped by the Beej’s guide. Some of the file management methods were referred to at https://fresh2refresh.com/c-programming/c-filehandling/fopen-fclose-gets-fputs-functions-c/ 
