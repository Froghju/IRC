#pragma once
#include "server.hpp"
class server;

class client {

private:
    client();

    int _clientId;
    sockaddr_in _clientInfo;
    socklen_t * _size;
    std::string _UserName;

public:
    client(int port);
    client &operator=(const client & src);
    ~client();

	sockaddr_in &SetClientInfo();
    void setClientName(std::string str);
	std::string GetClientUserName();
    sockaddr_in GetClientInfo() const;
    socklen_t * GetClientSize() const;
    struct pollfd InitPollFd(int fd);

    bool checkPollRevents(struct pollfd pipoll, std::vector<struct pollfd> *vec);

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
