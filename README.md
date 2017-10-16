# HTTPServer
Assignment 1 for the 3rd year Networks module.

## Compiling
The project can be compiled by executing

`$ make`

Clang is used for compilation.

## Current Status
Currently, there is a very simple server that is run by executing the
command

`$ ./server port`

where 'port' is an integer between 1024 and 65535.

The server will attempt to parse an incomming message as an HTTP request, and if
successful, it will echo some of the basic information from the request. It then
looks for the requested file and, if found, generates an appropriate HTTP
response and sends this, along with the file, back to the client.
