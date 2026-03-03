#pragma once
#include "server.hpp"

class server;

class client {

private:
    client();
    
    int _clientId;
    sockaddr_in _clientInfo;
    socklen_t * _size;

public:
    client(int port);
    client &operator=(const client & src);
    ~client();

	sockaddr_in &SetClientInfo();
    sockaddr_in GetClientInfo() const;
    socklen_t * GetClientSize() const;
    struct pollfd InitPollFd(int fd);

    void CheckPollRevents(struct pollfd pipo);

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