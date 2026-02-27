#include <netinet/in.h> // for sockaddr_in struct
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>

#include "class/client.hpp"
#include "class/server.hpp"

/*typedef struct s_Message {
    int size;
    int gId;
    char buff[4096];
}               t_Message;*/
