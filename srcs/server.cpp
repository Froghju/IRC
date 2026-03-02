#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port, std::string password) : _PassW(password), _Port(port)
{
	struct protoent *proto;

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

    _vpfd.fd = _IdSocket;
    _vpfd.events = POLLIN;
    _vpfd.revents = 0;

	listen(_IdSocket, 42); //nb de co possible en meme temps
}

struct pollfd server::GetPollFd() const
{
	return (_vpfd);
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

void server::checkPollRevents(std::vector<struct pollfd> *vec) 
{
	if ((*vec)[0].revents & POLLIN)
    {
        client cl(_Port);
		int fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
		if (fd_client != -1)
		{
			write(fd_client, "Hey I'm Tha_Ghj's serv\nI need the password for connection\n", 59);
			char buffer[_PassW.size() + 1];
			int check = 0;
			for (int i = 0; i < 3; i++)
			{
				check = read(fd_client, buffer, 2048);
				if (check == -1)
				{
					/*write(fd_client, "Error : can't read the password\n", 33);*/
					return;
				}
				buffer[check-1] = '\0';
				std::cout << buffer << std::endl;
				if (_PassW != buffer)
				{
					if (i + 1 < 3)
						write(fd_client, "Wrong password, try again\n", 27);
					else
					{
						write(fd_client, "Wrong password 3 time, I can't connect you\nBye Bye <3\n", 44);
						cl.~client();
						return;
					}
				}
				else
					break;
			}
			vec->push_back(cl.InitPollFd(fd_client));
			write(fd_client, "Welcome to Tha_Ghj's serv\n", 33);
		}
		else
		{
			std::cerr << "Error: can't accept connection" << std::endl;
		}
	}
	if ((*vec)[0].revents & POLLERR)
		std::cout << "erreur err" << std::endl;
	if ((*vec)[0].revents & POLLHUP)
		std::cout << "erreur hup" << std::endl;
}

server::~server()
{
	close(_IdSocket);
}