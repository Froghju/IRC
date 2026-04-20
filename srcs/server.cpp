#include "../libs/class/server.hpp"

server::server()
{
}

server::server(int port, std::string password) : _PassW(password), _Port(port), _Fro()
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
		std::cerr << "check init veccl Nickname " << _vecCl[i-1].GetNickname() << std::endl;
		std::cerr << "check init veccl UserName " << _vecCl[i-1].GetClientUserName() << std::endl;
		if (!_vecCl[i - 1].checkPollRevents((*vec)[i], vec, *this))
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
	//_Fro.hello(vec);
	//_Fro.frogsave(vec);
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

//privmsg ou join apres init
bool server::initNetcat(client &cl)
{
    send(cl.getOut(), "Please enter your user name:\n", 30, 0);
    std::string name = read_mess(cl.getOut());
    if (!name.empty())
    {
        for (std::vector<client>::iterator it = _vecCl.begin(); it != _vecCl.end(); ++it)
        {
            if (it->GetClientUserName() == name)
            {
                send(cl.getOut(), "Sorry username already used\nDisconnected from the server", 57, 0);
				return(false);
            }
        }
		cl.setClientName(name);
        send(cl.getOut(), "Please enter your nickname:\n", 29, 0);
    }
	else
		return false;
	std::string nickname = read_mess(cl.getOut());
	if (!nickname.empty())
	{
		for (std::vector<client>::iterator itt = _vecCl.begin(); itt != _vecCl.end(); ++itt)
		{
			if (itt->GetNickname() == name)
			{
				send(cl.getOut(), "Sorry nickname already used\nDisconnected from the server", 57, 0);
				return false;
			}
		}
	}
	else
		return false;
	cl.setNickname(nickname);
	send(cl.getOut(), "Welcome to Tha_Ghj's serv 🐸​!\n", 36, 0);
	return true;
}

bool server::initHex(client &cl)
{
	bool check = true;
	for (int i = 0; i < 2 && check; ++i)
	{
		std::string msg = read_mess(cl.getOut());
		std::cerr << "msg hex : " << msg << std::endl;
		if (!msg.empty())
		{
			std::string cmd = find_cmd(msg);
			std::cerr << "cmd : " << cmd << std::endl;
			std::string input = find_input(msg, cmd);
			std::cerr << "input : " << input << std::endl;
			if (cmd == "NICK")
			{
				if (!input.empty())
				{
					for (std::vector<client>::iterator itt = _vecCl.begin(); itt != _vecCl.end(); ++itt)
					{
						if (itt->GetNickname() == input)
						{
							send(cl.getOut(), "Sorry nickname already used\nDisconnected from the server\n", 58, 0);
							check = false;
							break;
						}
					}
					if (check)
					{
						cl.setNickname(input);
					}
				}
				else
				{
					check = false;
					send(cl.getOut(), "Bad nickname\nDisconnected from the server\n", 43, 0);
				}
			}
			else if (cmd == "USER")
			{
				if (!input.empty())
				{
					for (std::vector<client>::iterator it = _vecCl.begin(); it != _vecCl.end(); ++it)
					{
						if (it->GetClientUserName() == input)
						{
							send(cl.getOut(), "Sorry username already used\nDisconnected from the server\n", 58, 0);
							check = false;
							break;
						}
					}
					if (check)
						cl.setClientName(input);
				}
				else
				{
					check = false;
					send(cl.getOut(), "Bad username\nDisconnected from the server\n", 43, 0);
				}
			}
		}
		else
		{
			check = false;
			std::cerr << msg << std::endl;
		}
	}
	if (check)
	{
		std::string msg = ":localhost 001 " + cl.GetClientUserName() + " :Welcome to IRC server\r\n" + ":localhost 002 " + cl.GetClientUserName() + " :Your host is server\r\n" + ":localhost 003 " + cl.GetClientUserName() + " :This server was created today\r\n" + ":localhost 004 " + cl.GetClientUserName() + " server 1.0 o o\r\n";
		send(cl.getOut(), msg.c_str(), msg.size(), 0);
	}
	return check;
}

bool server::initUserNick(client &cl)
{
	bool check = true;
	if (cl.getHex())
		check = initHex(cl);
	else
		check = initNetcat(cl);
	return check;
}

void server::ExecCmd(std::vector<struct pollfd> *vec, client &cl, std::string mess)
{
	std::string cmd = find_cmd(mess);
	if (!cmd.empty())
	{
		if (cmd == "JOIN")
		{
			std::string channelName = findChannelName(mess, cmd);
			std::string clientName = findClientName(mess, cmd);
			joinCmd(channelName, clientName, cl);
		}
		else if (cmd == "KICK")
		{
			std::string channelName = findChannelName(mess, cmd);
			std::string clientName = findClientName(mess, cmd);
			kickCmd(channelName, cl, findClient(clientName));
		}
		else if (cmd == "INVITE")
		{
			std::string channelName = findChannelName(mess, cmd);
			std::string clientName = find_input(mess, cmd);
			inviteCmd(channelName, cl, findClient(clientName));
		}
		else if (cmd == "TOPIC")
		{
			std::string channelName = findChannelName(mess, cmd);
			std::string input = find_input(mess, cmd);
			topicCmd(channelName, input);
		}
		else if (cmd == "MODE")
		{
			std::string channelName = findChannelName(mess, cmd);
			modeCmd(findAllOptCmd(mess, cmd), cl);
		}
		else if (cmd == "Frogy")
			_Fro.hello(vec);
		else if (cmd == "FROG?" || cmd == "FUNFACT")
			_Fro.fact(vec);
		else if (cmd == "DRAW" || cmd == "FROGSAVE" || cmd == "MOTHER" || cmd == "EARTHBOUND")
			_Fro.frogsave(vec);
		else
		{
			if (cmd == "PRIVMSG")
			{
				std::string clientName = findClientName(mess, cmd);
				sendToClient(findClient(clientName), mess);
			}
			else
			{
				sendToAll(cl, vec, mess, *this);
				//a renplacer par un sendToAllChanel
			}
		}
	}
	else
	{
		std::cerr << "bad message" << std::endl;
	}
}
/*void server::parse(std::string message, client cl)
{
    std::vector<std::string> sentence = splitCpp(message);

    if (!message.empty())
    {
        if (cl.GetStep() < 3)
        {
            if (cl.GetStep() == 0)
            {
                if (sentence[0] == "PASS")
                    passCmd(cl);
            }
            else
            {
                if (sentence[0] == "USER")
                    userCmd();
                if (sentence[0] == "NICK")
                    nickCmd();
            }
        }
        if (sentence[0] == "JOIN")
        {
            //printf("cmd JOIN\n");
            if (sentence.size() > 2)
                joinCmd(sentence, cl);
            else
                send(cl.GetFdOut(), "Invalid command: JOIN <server_name>\n", 37, 0);
        }
        else if (sentence[0] == "INVITE")
        {
            printf("cmd INVITE\n");
        }
        else if (sentence[0] == "KICK")
        {
            printf("cmd KICK\n");
        }
        else if (sentence[0] == "TOPIC")
        {
            printf("cmd TOPIC\n");
        }
        else if (sentence[0] == "MODE")
        {
            printf("cmd MODE\n");
            if (sentence.size() > 1)
                modeCmd(sentence);
            else
                send(cl.GetFdOut(), "Invalid command: MODE <flag>\n", 30, 0);
        }
        else
        {
            printf("no parse\n");
        }
    }
}*/

bool server::Identification(std::vector<struct pollfd> *vec, client &cl)
{
	bool check  = true;
	std::string msg = read_mess(cl.getOut());
	if (!msg.empty())
	{
		std::string cmd = find_cmd(msg);
		if (!cmd.empty())
		{
			std::string input = find_input(msg, cmd);
			if (cmd == "CAP")
			{
				cl.setHex(true);
				msg = read_mess(cl.getOut());
				cmd = find_cmd(msg);
				input = find_input(msg, cmd);
				std::cerr << "cmd = " << cmd << std::endl;
				std::cerr << "input = " << input << std::endl;
			}
			if (cmd == "PASS")
			{

				if (input.empty() || (input != _PassW && input != _PassW + "\r"))
				{
					std::cerr << "bad password" << std::endl;
					check = false;
				}
			}
			else
			{
				std::cerr << "bad input not PASS cmd" << std::endl;
				check = false;
			}
		}
		else
			check = false;
	}
	else
		check = false;
	if (check)
		check = initUserNick(cl);
	if (check)
	{
		std::cerr << "check init cl Nickname " << cl.GetNickname() << std::endl;
		std::cerr << "check init cl UserName " << cl.GetClientUserName() << std::endl;
		_vecCl.push_back(cl);
        (*vec).push_back(cl.InitPollFd(cl.getOut()));
		//join serv par defaut ?
		std::cerr << "check init veccl Nickname " << _vecCl[_vecCl.size() - 1].GetNickname() << std::endl;
		std::cerr << "check init veccl UserName " << _vecCl[_vecCl.size() - 1].GetClientUserName() << std::endl;
	}
	else
	{
		shutdown(cl.getOut(), SHUT_RDWR);
		close(cl.getOut());
		std::cerr << "Client fail to connect" << std::endl;
	}
	return (check);
}