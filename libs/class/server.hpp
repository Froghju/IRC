#ifndef SERVER_HPP
#define SERVER_HPP

/*#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>*/

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cstring>
#include <exception>
#include "client.hpp"
#include <netdb.h>
#include <arpa/inet.h>

class server
{
private:
	server();
	int	_IdSocket;
	sockaddr_in	_InfServ;

public:
	server(int port, std::string password);
	int ConnectServ(client &cl);
	void WaitForConnectServ();
	void SerRecv(int IdConnectSocket);
	void SerSend(int IdConnectSocket);
	int	getIdSocket();
	~server();
};


#endif