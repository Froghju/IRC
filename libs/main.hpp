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

#include "class/client.hpp"
#include "class/server.hpp"
#include "frogy/frogy.hpp"

class client;
class frogy;

std::string read_mess(int fd);
char *strTochar(std::string str);
void sendToAll(int fd, std::vector<struct pollfd> *vec, std::string message, std::string name);
std::string find_cmd(std::string str);
std::string find_input(std::string str, std::string cmd);

#endif
