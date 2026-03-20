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

void server::returnPollClients(std::vector<struct pollfd> *vec)
{
	for (unsigned int i = 1; i < vec->size(); i++)
	{
		_vecCl[i - 1].checkPollRevents((*vec)[i]);
	}
}

void server::checkPollRevents(std::vector<struct pollfd> *vec) 
{
	if ((*vec)[0].revents & POLLIN)
    {
        client cl(_Port);
		int fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
		if (fd_client == -1)
		{
			std::cerr << "ERROR: can't accept connection" << std::endl;
		}
		else
		{
			send(fd_client, "Hey I'm Tha_Ghj's serv\nI need the password for connection:\n", 60, 0);
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
				if (_PassW != buffer)
				{
					if (i + 1 < 3)
						send(fd_client, "Wrong password, try again\n", 27, 0);
					else
					{
						send(fd_client, "Wrong password 3 time, I can't connect you\nBye Bye <3", 54, 0);
						shutdown(fd_client, SHUT_RDWR);
						close(fd_client);
						return;
					}
				}
				else
					break;
			}
			send(fd_client, "Welcome to Tha_Ghj's serv !\n", 29, 0);
			send(fd_client, "Please enter your user name\n", 29, 0);
			std::string all_text;
			while (1)
			{
				char buffer[2];
				int nb = read(fd_client, buffer, 1);
				if (nb == -1)
				{
					send(fd_client, "Sorry fail of recv you leave the serv\n", 39, 0);
					return;
				}
				if (nb == 0)
					break;
				buffer[nb] = '\0';
				all_text += buffer;
				std::cout << "check " << buffer << std::endl;
				std::cout << "check " << all_text << std::endl;
			}
			cl.setClientName(all_text);
			_vecCl.push_back(cl);
			(*vec).push_back(cl.InitPollFd(fd_client));
			std::cout << "user name is " << cl.GetClientUserName() << std::endl;
		}
		returnPollClients(vec);
	}
	if ((*vec)[0].revents & POLLERR)
		std::cerr << "erreur err" << std::endl;
	if ((*vec)[0].revents & POLLHUP)
		std::cerr << "erreur hup" << std::endl;
	(*vec)[0].revents = 0;
	if (vec->size() > 1)
	{
		int k = 1;
		for (std::vector<client>::iterator it = _vecCl.begin(); it != _vecCl.end(); it++)
		{
			it->checkPollRevents((*vec)[k]);
			k++;
		}
	}
}

std::vector<client> &server::getVecCl()
{
	return _vecCl;
}

server::~server()
{
	close(_IdSocket);
}