#include "../libs/class/server.hpp"

server::server() {}

server::server(int port, std::string password) : _ServName("Tha_Ghj"), _PassW(password), _Port(port), _Fro()
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
		if (!_vecCl[i - 1].checkPollRevents((*vec)[i], *this))
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

bool server::isvalidNickname(std::string input, client &cl)
{
	if (input.empty() || input == "\n" || input == "\r\n" || input[0] == '\0')
	{
		std::string ms = ":" + _ServName + " 431 :No nickname given\r\n";
		send(cl.getOut(), ms.c_str(), ms.size(), 0);
		return false;
	}
	if (!isalpha(input[0]))
	{
		std::string ms = ":" + _ServName + " 432 :Erroneus nickname\r\n";
		send(cl.getOut(), ms.c_str(), ms.size(), 0);
		return false;
	}
	if (input.size() > 9)
	{
		std::string ms = ":" + _ServName + " 432 :Erroneus nickname\r\n";
		send(cl.getOut(), ms.c_str(), ms.size(), 0);
		return false;
	}
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (!isalnum(input[i]) && input[i] != '-' && input[i] != '_' && input[i] != '[' && input[i] != ']' && input[i] != '\\' && input[i] != '^' && input[i] != '{' && input[i] != '}' && input[i] != '|' && (i == input.size() - 2 && input[i] == '\r'))
		{
			std::string ms = ":" + _ServName + " 432 :Erroneus nickname\r\n";
			send(cl.getOut(), ms.c_str(), ms.size(), 0);
			return false;
		}
	}
	std::vector<client>::iterator itt = _vecCl.begin();
	while (itt != _vecCl.end())
	{
		if (itt->GetNickname() == input)
		{
			std::string ms = ":" + _ServName + " 433 :Nickname is already in use\r\n";
			send(cl.getOut(), ms.c_str(), ms.size(), 0);
			return false;
		}
		++itt;
	}
	return (true);
}

bool server::isvalidUsername(std::string input, client &cl)
{
	if (input.empty() || input == "\n" || input == "\r\n" || input[0] == '\0')
	{
		std::string ms = ":" + _ServName + " 461 USER :Not enough parameters\r\n";
		send(cl.getOut(), ms.c_str(), ms.size(), 0);
		return false;
	}
	if (!isalpha(input[0]))
		return false;
	for (size_t i = 0; i < input.size(); ++i)
	{
		if (!isalnum(input[i]) && input[i] != '-' && input[i] != '_' && input[i] != '.' && (i == input.size() - 2 && input[i] == '\r'))
			return false;
	}
	return (true);
}

client &server::findClient(std::string clientNick)
{
	size_t i = 0;
	std::cerr << "clientNick = " << clientNick << std::endl;
	while (i < _vecCl.size())
	{
		if (_vecCl[i].GetNickname() == clientNick || _vecCl[i].GetNickname() == ":"+clientNick)
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
		send(findClient(content[0]).getOut(), str.c_str(), str.size(), 0);
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
	if (!content[0].empty())
	{
		if (content.size() > 1)
		{
			if (content[0] == "JOIN")
				joinCmd(content, cl);
			else if (content[0] == "KICK")
				kickCmd(content, cl);
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
			{
				try {
					std::cerr << "check1" << std::endl;
					size_t i = findChannel(content[1]);
					std::cerr << "check2" << std::endl;
					if (_vecCh[i].isOnTheChannel(cl))
					{
						_vecCh[i].sendToAll(cl, content);
						std::cerr << "check3" << std::endl;
					}
					else
					{
						std::string str = "Join channel to talk to people\n";
						send(cl.GetFdOut(), str.c_str(), str.size(), 0);
						str.clear();
					}
				}
				catch(const std::exception& e)
				{
					std::cerr << "check4" << std::endl;
					(void)e;
					sendToClient(content);
				}
			}
		}
	}
	else
	{
		std::cerr << "bad message" << std::endl;
	}
}

std::string server::usernamehexchat(std::string &input)
{
	size_t pos = input.find(" 0");
	return input.substr(0, pos);
}

bool server::Identification(std::vector<struct pollfd> *vec, client &cl)
{
	bool check = false;
	bool pass = false;
	bool nick = false;
	bool user = false;
	while (!check)
	{
		while (!pass)
		{
			std::string msg = read_mess(cl.getOut());
			if (!msg.empty())
			{
				std::string cmd = find_cmd(msg);
				if (!cmd.empty())
				{
					if (cmd == "PASS")
					{
						std::string input = find_input(msg, cmd);
						if (!input.empty() && (input == _PassW || input == _PassW + "\r"))
							pass = true;
						else
						{
							std::string ms = ":" + _ServName + " 464 :Password incorrect\r\n";
							send(cl.getOut(), ms.c_str(), ms.size(), 0);
						}
					}
					else if (cmd == "CAP")
					{
						std::string ms = ":" + _ServName + " CAP * LS :\r\n";
						send(cl.getOut(), ms.c_str(), ms.size(), 0);
					}
				}
			}
		}
		if (pass)
		{
			std::string msg = read_mess(cl.getOut());
			if (!msg.empty() && msg != "\n" && msg != "\r\n" && msg[0] != '\0')
			{
				std::string cmd = find_cmd(msg);
				std::string input = find_input(msg, cmd);
				if (cmd == "NICK")
				{
					std::cerr << "input = " << input << std::endl;
					if (input[input.size() - 1] == '\r')
					{
						std::string str;
						for (size_t i = 0; i < input.size() - 1; ++i)
						{
							str += input[i];
						}
						input.clear();
						input = str;
					}
					if (isvalidNickname(input, cl))
					{
						cl.setNickname(input);
						for (size_t i = 0; i < cl.GetNickname().size(); i++)
						{
							std::cout << (int)(unsigned char)cl.GetNickname()[i] << " ";
						}
						std::cout << std::endl;
						nick = true;
					}
				}
				else if (cmd == "USER")
				{
					if (isvalidUsername(input, cl))
					{
						std::string onlyuser = usernamehexchat(input);
						cl.setClientName(onlyuser);
						user = true;
					}
				}
				else if (cmd == "PASS")
				{
					std::string ms = ":" + _ServName + " 462 :Unauthorized command\r\n";
					send(cl.getOut(), ms.c_str(), ms.size(), 0);
				}
				if (user && nick)
					check = true;
			}
		}
	}
	std::string msg = ":localhost 001 " + cl.GetNickname() + " :Welcome to " + _ServName + "\r\n" + ":localhost 002 " + cl.GetNickname() + " :Your host is " + _ServName + "\r\n" + ":localhost 003 " + cl.GetNickname() + " :This server was created today\r\n" + ":localhost 004 " + cl.GetNickname() + " server 1.0 o o\r\n";
	send(cl.getOut(), msg.c_str(), msg.size(), 0);
	_vecCl.push_back(cl);
	(*vec).push_back(cl.InitPollFd(cl.getOut()));
	return (check);
}