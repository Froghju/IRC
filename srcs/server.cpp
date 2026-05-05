#include "../libs/class/server.hpp"

server::server() {}

server::server(int port, std::string password) : _PassW(password), _Port(port), _Fro()
{
	struct protoent *proto;

	proto = getprotobyname("tcp");//check si pas tcp/ip
	if (proto == 0)
		std::cerr << "Protobyname error" << std::endl;
	//_IdSocket = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	_IdSocket = socket(AF_INET, SOCK_STREAM, 0); //*
	fcntl(_IdSocket, F_SETFL, O_NONBLOCK);
	if (_IdSocket == -1)
		std::cerr << "Socket error" << std::endl;

	std::memset(&_InfServ, 0, sizeof(_InfServ));  //*
	_InfServ.sin_family = AF_INET;
	_InfServ.sin_port = htons(_Port); /// htons host to network short
	_InfServ.sin_addr.s_addr = INADDR_ANY; //peut se connecter de partout
	if (bind(_IdSocket, (const struct sockaddr*)&_InfServ, sizeof(_InfServ)))
	{
		std::cerr << "Inable to connect, port already used" << std::endl;
		throw ErrorQuit();
	}

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
		if (!_vecCl[i - 1].checkPollRevents((*vec)[i], *this))
		{
			shutdown((*vec)[i].fd, SHUT_RDWR);
			(*vec).erase((*vec).begin() + i);
			_vecCl.erase(_vecCl.begin() + i - 1);
		}
	}
}

/*bool server::checkPassword(int fd)
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
}*/

void server::checkPollRevents(std::vector<struct pollfd> *vec)
{
	if ((*vec)[0].revents & POLLIN)
    {
        client cl(_Port);
		socklen_t len = sizeof(cl.GetClientInfo());
		int fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), &len);
		if (fd_client == -1)
			std::cerr << "ERROR: can't accept connection" << std::endl;
		else
		{
			cl.setOut(fd_client);
			if (!Identification(vec, cl))
				std::cerr << "Client fail to connect" << std::endl;
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

bool server::isvalidNickname(std::string input)
{
	if (input.empty() || input == "\n" || input == "\r\n" || input[0] == '\0')
		return false;
	if (!isalpha(input[0]))
		return false;
	if (input.size() > 9)
		return false;
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (!isalnum(input[i]) && input[i] != '-' && input[i] != '_' && input[i] != '[' && input[i] != ']' && input[i] != '\\' && input[i] != '^' && input[i] != '{' && input[i] != '}' && input[i] != '|')
			return false;
	}
	std::vector<client>::iterator itt = _vecCl.begin();
	while (itt != _vecCl.end())
	{
		if (itt->GetNickname() == input)
		{
			//send(cl.getOut(), "Sorry nickname already used\nDisconnected from the server\n", 58, 0);
			return false;
		}
		++itt;
	}
	return (true);
}

bool server::isvalidUsername(std::string input)
{
	if (input.empty() || input == "\n" || input == "\r\n" || input[0] == '\0')
		return false;
	if (!isalpha(input[0]))
		return false;
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (!isalnum(input[i]) && input[i] != '-' && input[i] != '_' && input[i] != '.')
			return false;
	}
	return (true);
}

bool server::initClient(client &cl)
{
	bool check = false;
	bool nick = false;
	bool user = false;
	while (!check)
	{
		std::string msg = read_mess(cl);
		if (!msg.empty() && msg != "\n" && msg != "\r\n" && msg[0] != '\0')
		{
			std::string cmd = find_cmd(msg);
			std::string input = find_input(msg, cmd);
			if (cmd == "NICK")
			{
				if (isvalidNickname(input))
				{
					cl.setNickname(input);
					nick = true;
					std::cerr << YELLOW << "[log]: Nickname register" << RESET << std::endl; 
				}
			}
			else if (cmd == "USER")
			{
				if (isvalidUsername(input))
				{
					cl.setClientName(input);
					user = true;
					std::cerr << YELLOW << "[log]: Username register" << RESET << std::endl; 
				}
			}
			if (user && nick)
				check = true;
		}
	}
	std::string msg = ":localhost 001 " + cl.GetClientUserName() + " :Welcome to IRC server\r\n" + ":localhost 002 " + cl.GetClientUserName() + " :Your host is server\r\n" + ":localhost 003 " + cl.GetClientUserName() + " :This server was created today\r\n" + ":localhost 004 " + cl.GetClientUserName() + " server 1.0 o o\r\n";
	send(cl.getOut(), msg.c_str(), msg.size(), 0);
	return check;
}

client &server::findClient(std::string clientNick)
{
	size_t i = 0;
	while (i < _vecCl.size())
	{
		if (_vecCl[i].GetNickname() == clientNick)
			return _vecCl[i];
		i++;
	}
	throw ClientNotFound();
}

void server::sendToClient(std::vector<std::string> content)
{
	try
	{
		if (content.size() < 3)
			throw NoMessage();
		std::string str;
		for (size_t i = 2; i < content.size(); i++)
		{
			str += content[i];
			if (i + 1 < content.size())
				str+= " ";
		}
		str += "\r\n";
		send(findClient(content[1]).getOut(), str.c_str(), str.size(), 0);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

void server::ExecCmd(client &cl, std::string mess)
{
	std::cerr << "mess = " << mess << std::endl;
	std::vector<std::string> content = splitCpp(mess);
	/*size_t j = 0;
    while (j < content.size())
    {
        std::cout << "result: .." << content[j] << ".." << std::endl;
        j++;
    }*/
	if (!content[0].empty())
	{
		if (content.size() > 1)
		{
			if (content[0] == "JOIN")
				joinCmd(content, cl);
			else if (content[0] == "KICK")
			{
				std::cout << "kick :" << cl.GetOperator();
				kickCmd(content, cl);
			}
			else if (content[0] == "INVITE")
				inviteCmd(content, cl);
			else if (content[0] == "TOPIC")
				topicCmd(content, cl);
			else if (content[0] == "MODE")
				modeCmd(content, cl);
			else if (content[0] == "Frogy")
			{
				size_t i = findChannel(content[1]);
				_Fro.hello(_vecCh[i]);
			}
			else if (content[0] == "FROG?" || content[0] == "FUNFACT")
			{
				size_t i = findChannel(content[1]);
				_Fro.fact(_vecCh[i]);
			}
			else if (content[0] == "DRAW" || content[0] == "FROGSAVE" || content[0] == "MOTHER" || content[0] == "EARTHBOUND")
			{
				size_t i = findChannel(content[1]);
				_Fro.frogsave(_vecCh[i]);
			}
			else if (content[0] == "PRIVMSG")
				sendToClient(content);
			else
			{
				try
				{
					size_t i = findChannel(content[0]);
					if (_vecCh[i].isOnTheChannel(cl))
						_vecCh[i].sendToAll(cl, mess);
					else
					{
						std::string str = "Join channel to talk to people\n";
						send(cl.GetFdOut(), str.c_str(), str.size(), 0);
						str.clear();
					}
				}
				catch(const std::exception& e)
				{
					(void)e;
					std::string str = "Join channel to talk to people\n";
					send(cl.GetFdOut(), str.c_str(), str.size(), 0);
					str.clear();
				}
			}
		}
	}
	else
	{
		std::cerr << "bad message" << std::endl;
	}
}


bool server::Identification(std::vector<struct pollfd> *vec, client &cl)
{
	bool check  = false;
	bool pass = false;
	bool nick = false;
	bool user = false;
	while (!check)
	{
		while (!pass)
		{
			std::string msg = read_mess(cl);
			if (!msg.empty())
			{
				std::string cmd = find_cmd(msg);
				if (!cmd.empty())
				{
					if (cmd == "PASS")
					{
						std::string input = find_input(msg, cmd);
						if (!input.empty() && (input == _PassW || input == _PassW + "\r"))
						{
							pass = true;
							std::cerr << YELLOW << "[log]: Password register" << RESET << std::endl; 
						}
					}
				}
			}
		}
		if (pass)
		{
			std::string msg = read_mess(cl);
			if (!msg.empty() && msg != "\n" && msg != "\r\n" && msg[0] != '\0')
			{
				std::string cmd = find_cmd(msg);
				std::string input = find_input(msg, cmd);
				if (cmd == "NICK")
				{
					if (isvalidNickname(input))
					{
						cl.setNickname(input);
						nick = true;
						std::cerr << YELLOW << "[log]: Nickname register" << RESET << std::endl; 
					}
				}
				else if (cmd == "USER")
				{
					if (isvalidUsername(input))
					{
						cl.setClientName(input);
						user = true;
						std::cerr << YELLOW << "[log]: Username register" << RESET << std::endl;
					}
				}
				if (user && nick)
					check = true;
			}
		}
	}
	std::string msg = ":localhost 001 " + cl.GetClientUserName() + " :Welcome to IRC server\r\n" + ":localhost 002 " + cl.GetClientUserName() + " :Your host is server\r\n" + ":localhost 003 " + cl.GetClientUserName() + " :This server was created today\r\n" + ":localhost 004 " + cl.GetClientUserName() + " server 1.0 o o\r\n";
	send(cl.getOut(), msg.c_str(), msg.size(), 0);
	_vecCl.push_back(cl);
	(*vec).push_back(cl.InitPollFd(cl.getOut()));
	return (check);
}