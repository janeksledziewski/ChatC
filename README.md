# ChatC
Console chat writen in C using POSIX Sockets.
Main.c
Tested with gcc, on Ubuntu.
Creates server - with the given port passed by arguments. Number of clients is defined by variable. Currently set to 10.

client.c
Tested with gcc, on Ubuntu.
Client program - uses pthread to read the messages from server.
