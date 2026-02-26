#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port, std::string password) : _PassW(password), _Port(port)
{
	struct protoent *proto;
	int fd_client;

	proto = getprotobyname("tcp");//check si pas tcp/ip
	if (proto == 0)
		std::cerr << "oupsi pas bon" << std::endl;
	_IdSocket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (_IdSocket == -1)
		std::cerr << "oupsi pas bon" << std::endl;

	_InfServ.sin_family = AF_INET;
	_InfServ.sin_port = htons(_Port); /// htons host to network short
	_InfServ.sin_addr.s_addr = INADDR_ANY; //peut se connecter de partout
	if (bind(_IdSocket, (const struct sockaddr*)&_InfServ, sizeof(_InfServ)))
		std::cerr << "oupsi pas bon" << std::endl;

	listen(_IdSocket, 42); //nb de co possible en meme temps
	client cl(_Port);
	fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
	char buffer[15];
	int nb = read(fd_client, buffer, 14);
	buffer[nb] = '\0';
	std::cout << buffer;
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

/*int server::ConnectServ(client &cl)
{
	unsigned int size = sizeof(cl.GetClientInfo());
	int IdConnectSocket = accept(_IdSocket, (sockaddr *)&cl.GetClientInfo(), &size);
	if (IdConnectSocket == -1)
		std::cerr << "oupsi pas bon" << std::endl;
	return (IdConnectSocket);
}*/

/*void server::SerRecv(int IdConnectSocket)
{

}

void server::SerSend(int IdConnectSocket)
{

}*/
// _clientInfo.sin_port = htons(port);

server::~server()
{
	close(_IdSocket);
}