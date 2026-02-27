#ifndef SERVER_HPP
#define SERVER_HPP

/*#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>*/

/*#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cstring>
#include <exception>
#include "client.hpp"
#include <netdb.h>
#include <arpa/inet.h>*/

#include "../main.hpp"

class client;

class server
{
private:
	server();

	std::string _PassW;
	int _Port;
	int	_IdSocket;

	sockaddr_in	_InfServ;
	struct pollfd _vpfd;

public:
	server(int port, std::string password);
	struct pollfd GetPollFd() const;
	void WaitForConnectServ();
	int	getIdSocket();
	void checkPollRevents(std::vector<struct pollfd> *vec);
	~server();
};

#endif