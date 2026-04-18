#ifndef MAIN_HPP
#define MAIN_HPP

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
#include <algorithm>

#include "class/client.hpp"
#include "class/server.hpp"
#include "class/channel.hpp"

//debug
#include <stdio.h>

class client;

//utils.cpp
std::string read_mess(int fd);
char *strTochar(std::string str);
void sendToAll(int fd, std::vector<struct pollfd> *vec, std::string message, std::string name);
std::vector<std::string> splitCpp(std::string str);
std::map<int, std::string> initDataBot();

#endif