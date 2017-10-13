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

The server can be connected to using, for example telnet:

`$ telnet host port`

or netcat:

`$ nc host port`

Messages can be typed, and they will be echoed by the server.

The server is multithreaded, and can therefore handle multiple simultaneous
connections.
