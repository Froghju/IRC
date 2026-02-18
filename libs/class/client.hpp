#pragma once
#include "server.hpp"

class server;

class client {

private:
    client();
    
    int _clientSocket;
    sockaddr_in _clientInfo;

public:
    client(int port);
    ~client();
	sockaddr_in getClientInfo();
};