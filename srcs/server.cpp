#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port)
{
	_IdSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_IdSocket == -1)
		throw (errno);
	int tmp = 1;
	if (setsockopt(_IdSocket, IPPROTO_TCP, TCP_NODELAY, &tmp, sizeof(tmp)) != 0)
		throw (errno); //check si setsockopt renvoi bien dans ernno son erreur
	_InfServ.sin_family = _IdSocket;
	_InfServ.sin_addr.s_addr = INADDR_ANY;
	_InfServ.sin_port = htons(port);
	if (bind(_IdSocket, (struct sockaddr*)&_InfServ, sizeof(_InfServ)))
		throw (errno); //check si ok
}

void server::WaitForConnectServ()
{
	int check = -1;
	while(check != 0)
		check = listen(_IdSocket, 1);
}

int server::ConnectServ(client &cl)
{
	unsigned int tmp = sizeof(cl.getClientInfo());
	int IdConnectSocket = accept(_IdSocket, (sockaddr *)&cl.getClientInfo(), &tmp);
	if (IdConnectSocket == -1)
		throw (errno);
	return (IdConnectSocket);
}

void server::SerRecv(int IdConnectSocket)
{

}

void server::SerSend(int IdConnectSocket)
{

}
// _clientInfo.sin_port = htons(port);

server::~server()
{
}