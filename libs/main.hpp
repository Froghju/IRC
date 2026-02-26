#include <netinet/in.h> // for sockaddr_in struct
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <string.h>

struct Message {
    int size;
    int gId;
    char buff[4096];
};