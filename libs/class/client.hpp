#pragma once
#include "server.hpp"

class server;

class client {

private:
    client();
    
    int _clientId;
    int _clientFd;
    sockaddr_in _clientInfo;

public:
    client(int port);
    ~client();
	sockaddr_in GetClientInfo();

    void ClientConnect(server &svr);
    void ClientSend(server &svr);
    void ClientRecv(server &svr);
    void WaitForReponseServ();

    class InvalidClientSig : public std::exception {
        public:
            const char *_error;

            InvalidClientSig(const char *data) : _error(data) {}
            virtual const char* what() const throw()
            {
                return (_error);
            }
    };
};