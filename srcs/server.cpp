#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port, std::string password) : _PassW(password), _Port(port)
{
	struct protoent *proto;

	proto = getprotobyname("tcp");//check si pas tcp/ip
	if (proto == 0)
		std::cerr << "Protobyname error" << std::endl;
	_IdSocket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	if (_IdSocket == -1)
		std::cerr << "Socket error" << std::endl;

	_InfServ.sin_family = AF_INET;
	_InfServ.sin_port = htons(_Port); /// htons host to network short
	_InfServ.sin_addr.s_addr = INADDR_ANY; //peut se connecter de partout
	if (bind(_IdSocket, (const struct sockaddr*)&_InfServ, sizeof(_InfServ)))
		std::cerr << "Inable to connect, port already used" << std::endl;

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
		if (!_vecCl[i - 1].checkPollRevents((*vec)[i], vec, this))
		{
			shutdown((*vec)[i].fd, SHUT_RDWR);
			(*vec).erase((*vec).begin() + i);
			_vecCl.erase(_vecCl.begin() + i - 1);
		}
	}
}

bool server::checkPassword(int fd)
{
	std::string mess;
	for (int i = 0; i < 3; i++)
	{
		mess = read_mess(fd);
		if (mess.empty())
			return false;
		if (_PassW != mess)
		{
			if (i + 1 < 3)
				send(fd, "Wrong password, try again\n", 27, 0);
			else
			{
				send(fd, "Wrong password 3 time, I can't connect you\nBye Bye <3", 54, 0);
				shutdown(fd, SHUT_RDWR);
				close(fd);
				return false;
			}
		}
		else
			break;
	}
	return true;
}

void server::checkPollRevents(std::vector<struct pollfd> *vec)
{
	if ((*vec)[0].revents & POLLIN)
    {
        client cl(_Port);
		int fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
		if (fd_client == -1)
			std::cerr << "ERROR: can't accept connection" << std::endl;
		else
		{
			cl.setFdOut(fd_client);
			send(fd_client, "Hey ! I'm Tha_Ghj's serv 🐸\n", 31, 0);
			//passCmd(cl);
			Identification(fd_client, vec, cl);
			printf("C\n");
		}
	}
	if ((*vec)[0].revents & POLLERR)
		std::cerr << "erreur err" << std::endl;
	if ((*vec)[0].revents & POLLHUP)
		std::cerr << "erreur hup" << std::endl;
	(*vec)[0].revents = 0;
	returnPollClients(vec);
}

std::vector<client> &server::getVecCl()
{
	return _vecCl;
}

server::~server()
{
	close(_IdSocket);
}

void server::Identification(int fd_client, std::vector<struct pollfd> *vec, client cl)
{
    std::string cmd = read_mess(fd_client);
	(void)vec;

	/*if (!cmd.empty())
	{
		std::vector<std::string> pass = splitCpp(cmd);
		if (pass.size() == 2 && (pass[0] == "USER" || pass[0] == "NICK"))
		{
			if (pass[0] == "USER")
				cl.setClientName(pass[1]);
			else
				cl.setNickname(pass[1]);
		}
		while (cl.GetClientUserName().empty() || cl.GetNickname().empty())
			Identification(fd_client, vec, cl);
	}*/
	parse(cmd, cl);

    /*if (!name.empty())
    {
        for (std::vector<client>::iterator it = _vecCl.begin(); it != _vecCl.end(); ++it)
        {
            if (it->GetClientUserName() == name)
            {
                send(fd_client, "Sorry username already used\nDisconnected from the server", 57, 0);
                shutdown(fd_client, SHUT_RDWR);
                close(fd_client);
                return;
            }
        }
        send(fd_client, "Please enter your nickname:\n", 29, 0);
        std::string nickname = read_mess(fd_client);
        if (!nickname.empty())
        {
            for (std::vector<client>::iterator itt = _vecCl.begin(); itt != _vecCl.end(); ++itt)
            {
                if (itt->GetNickname() == name)
                {
                    send(fd_client, "Sorry nickname already used\nDisconnected from the server", 57, 0);
                    shutdown(fd_client, SHUT_RDWR);
                    close(fd_client);
                    return;
                }
            }
        }
        cl.setClientName(name);
        cl.setNickname(nickname);
        _vecCl.push_back(cl);
        (*vec).push_back(cl.InitPollFd(fd_client));
        std::cout << "New user " << cl.GetClientUserName() << " join Tha_Ghj serv" << std::endl;
        send(fd_client, "Successful connection! Please join a channel with the command 'JOIN'\n", 70, 0);
    }*/
	// CKECk de USERNAME et NICKNAME
    /*else
        std::cerr << "Client disconnected" << std::endl;*/
}