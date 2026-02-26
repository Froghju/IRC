#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port, std::string password)
{
	struct protoent *proto;

	proto = getprotobyname("tcp");//check si pas tcp/ip
	if (proto == 0)
		throw (errno);
	_IdSocket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (_IdSocket == -1)
		throw (errno);
	/*int tmp = 1;
	if (setsockopt(_IdSocket, IPPROTO_TCP, TCP_NODELAY, &tmp, sizeof(tmp)) != 0)
		throw (errno); //check si setsockopt renvoi bien dans ernno son erreur*/
	_InfServ.sin_family = AF_INET;
	_InfServ.sin_port = htons(port); /// htons host to network short
	_InfServ.sin_addr.s_addr = INADDR_ANY; //peut se connecter de partout
	if (bind(_IdSocket, (const struct sockaddr*)&_InfServ, sizeof(_InfServ)))
		throw (errno); //check si ok
	listen(_IdSocket, 42); //nb de co possible en meme temps
}

int	server::getIdSocket()
{
	return (_IdSocket);
}

void server::WaitForConnectServ()
{
	int check = -1;
	while(check != 0)
		check = listen(_IdSocket, 1);
}

int server::ConnectServ(client &cl)
{
	unsigned int size = sizeof(cl.GetClientInfo());
	int IdConnectSocket = accept(_IdSocket, (sockaddr *)&cl.GetClientInfo(), &size);
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