#include "../libs/class/server.hpp"

server::server() {}

server::server(int port, std::string password) : _ServName("Tha_Ghj"), _PassW(password), _Port(port), _Fro()
{
	struct protoent *proto;

	proto = getprotobyname("tcp");//check si pas tcp/ip
	if (proto == 0)
		std::cerr << "Protobyname error" << std::endl;
	_IdSocket = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_IdSocket, F_SETFL, O_NONBLOCK);
	if (_IdSocket == -1)
		std::cerr << "Socket error" << std::endl;

	std::memset(&_InfServ, 0, sizeof(_InfServ));
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

int	server::getIdSocket() const
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
		if (!_vecCl[i - 1].checkPollRevents(vec, i, *this))
		{
			shutdown((*vec)[i].fd, SHUT_RDWR);
			(*vec).erase((*vec).begin() + i);
			_vecCl.erase(_vecCl.begin() + i - 1);
		}
	}
}

void server::checkPollRevents(std::vector<struct pollfd> *vec)
{
	if ((*vec)[0].revents & POLLERR)
		std::cerr << "erreur err" << std::endl;
	if ((*vec)[0].revents & POLLHUP)
		std::cerr << "erreur hup" << std::endl;
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
			_vecCl.push_back(cl);
			(*vec).push_back(cl.InitPollFd(cl.getOut()));
		}
	}
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
	if (input == "Frogy")
	{
		std::string ms = ":" + _ServName + " 433 :Nickname is already in use\r\n";
		send(cl.getOut(), ms.c_str(), ms.size(), 0);
		return false;
	}
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

void server::deleteClient(client &cl)
{
	shutdown(cl.GetClientID(), SHUT_RDWR);
    close(cl.GetClientID());
	_vecCl.erase(std::find(_vecCl.begin(), _vecCl.end(), cl));
}

client &server::findClient(std::string clientNick)
{
	size_t i = 0;
	while (i < _vecCl.size())
	{
		if (_vecCl[i].GetNickname() == clientNick || _vecCl[i].GetNickname() == ":"+clientNick)
			return _vecCl[i];
		i++;
	}
	throw ClientNotFound();
}

void server::sendToClient(std::vector<std::string> content, client &cl)
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
		std::string hex_mess = ":" + cl.GetNickname() +
					"!~" + cl.GetClientUserName() +
					"@localhost " + content[0] + " " + content[1] + " " +
					str;
		send(findClient(content[1]).getOut(), hex_mess.c_str(), hex_mess.size(), 0);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

void server::ExecCmd(client &cl, std::string mess)
{
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
			else if (content[0] == "NICK")
			{
				if (content[1][content[1].size() - 1] == '\r')
				{
					std::string str;
					for (size_t i = 0; i < content[1].size() - 1; ++i)
					{
						str += content[1][i];
					}
					content[1].clear();
					content[1] = str;
				}
				if (isvalidNickname(content[1], cl))
				{
					sendNewNick(cl, content[1]);
					for (size_t j = 0; j < _vecCh.size(); ++j)
					{
						for (size_t i = 0; i < _vecCh[j].getchannelClients().size(); ++i)
						{
							if (_vecCh[j].getchannelClients()[i] == cl)
							{
								std::string mess = ":" + cl.GetNickname() + "!" + cl.GetClientUserName() + "@localhost NICK :" + content[1] + "\r\n";
								for (std::vector<client>::iterator it = _vecCh[j].getchannelClients().begin(); it != _vecCh[j].getchannelClients().end(); it++)
								{
									send(it->getOut(), mess.c_str(), mess.size(), 0);
								}
								if (_vecCh[j].isAdmin(cl))
								{
									for (size_t k = 0; k < _vecCh[j].getchannelAdmin().size(); ++k)
									{
										if (_vecCh[j].getchannelAdmin()[k] == cl)
										{
											_vecCh[j].getchannelAdmin()[i].setNickname(content[1]);
											break;
										}
									}
								}
								_vecCh[j].getchannelClients()[i].setNickname(content[1]);
							}
							if (_vecCh[j].getchannelList()[i] == cl)
							{
								for (size_t k = 0; k < _vecCh[j].getchannelList().size(); ++k)
								{
									if (_vecCh[j].getchannelList()[k] == cl)
									{
										_vecCh[j].getchannelList()[i].setNickname(content[1]);
										break;
									}
								}
							}
						}
					}
					cl.setNickname(content[1]);
				}
			}
			else if (content[0] == "PRIVMSG")
			{
				try {
					size_t i = findChannel(content[1]);
					if (_vecCh[i].isOnTheChannel(cl))
					{
						_vecCh[i].sendToAll(cl, content);
						if (content.size() > 2)
						{
							if (content[2] == ":Frogy")
							{
								_Fro.hello(_vecCh[i], content);
							}
							else if (content[2] == ":FROG?" || content[2] == ":FUNFACT")
							{
								_Fro.fact(_vecCh[i], content);
							}
							else if (content[2] == ":DRAW" || content[2] == ":FROGSAVE" || content[2] == ":MOTHER" || content[2] == ":EARTHBOUND")
							{
								_Fro.frogsave(_vecCh[i], content);
							}
						}
					}
					else
					{
						std::string str = "Join channel to talk to people\n";
						send(cl.getOut(), str.c_str(), str.size(), 0);
						str.clear();
					}
				}
				catch(const std::exception& e)
				{
					(void)e;
					sendToClient(content, cl);
				}
			}
		}
	}
	/*else
	{
		std::cerr << "bad message" << std::endl;
	}*/
}

std::string server::usernamehexchat(std::string &input)
{
	size_t pos = input.find(" 0");
	return input.substr(0, pos);
}

void server::Identification(client &cl, std::string msg)
{
	try
	{
		std::string cmd = find_cmd(msg);
		if (!cmd.empty())
		{
			std::string input = find_input(msg, cmd);
			if (!cl.GetPass() && !input.empty())
			{
				if (cmd == "PASS")
				{
					if (!input.empty() && (input == _PassW || input == _PassW + "\r"))
					{
						cl.setReady('P');
					}
				}
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
			else
			{
				if (!input.empty() && msg != "\n" && msg != "\r\n" && msg[0] != '\0')
				{
					if (cmd == "NICK")
					{
						if (input[input.size() - 1] == '\r')
						{
							std::string str;
							for (size_t i = 0; i < input.size() - 1; ++i)
								str += input[i];
							input.clear();
							input = str;
						}
						if (isvalidNickname(input, cl))
						{
							cl.setNickname(input);
							cl.setReady('N');
						}
					}
					else if (cmd == "USER")
					{
						if (isvalidUsername(input, cl))
						{
							std::string onlyuser = usernamehexchat(input);
							cl.setClientName(onlyuser);
							cl.setReady('U');
						}
					}
					else if (cmd == "PASS")
					{
						std::string ms = ":" + _ServName + " 462 :Unauthorized command\r\n";
						send(cl.getOut(), ms.c_str(), ms.size(), 0);
					}
				}
			}
		}
		if (cl.GetReady())
		{
			std::string enter = ":localhost 001 " + cl.GetNickname() + " :Welcome to " + _ServName + "\r\n" + ":localhost 002 " + cl.GetNickname() + " :Your host is " + _ServName + "\r\n" + ":localhost 003 " + cl.GetNickname() + " :This server was created today\r\n" + ":localhost 004 " + cl.GetNickname() + " server 1.0 o o\r\n";
			send(cl.getOut(), enter.c_str(), enter.size(), 0);
		}
	}
	catch (const std::exception& e)
	{
		(void)e;
		deleteClient(cl);
	}
}