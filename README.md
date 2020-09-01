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
