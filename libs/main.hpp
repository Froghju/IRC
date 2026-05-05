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
#include <fstream>
#include <map>
#include <deque>
#include <bits/stdc++.h>
#include <fcntl.h>

#include "class/client.hpp"
#include "class/server.hpp"
#include "frogy/frogy.hpp"
#include "class/channel.hpp"

//debug
#include <stdio.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

class client;
class frogy;
class server;

//utils.cpp
std::string read_mess(client &cl);
char *strTochar(std::string str);
void sendToAll(client &cl, std::vector<struct pollfd> *vec, std::string message, server &serv);
std::string find_cmd(std::string str);
std::string find_input(std::string str, std::string cmd);
std::vector<std::string> splitCpp(std::string str);
std::vector<std::string> initDataBot();

class ErrorQuit : public std::exception {

public:

    virtual const char* what() const throw()
    {
        return ("\nYou quit the server. ByeBye <3");
    }
};

#endif