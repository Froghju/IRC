#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.hpp"


class client;
class channel;

class server
{
private:
	server();

	std::string _PassW;
	int _Port;
	int	_IdSocket;

	sockaddr_in	_InfServ;
	struct pollfd _vpfd;
	std::vector<client>	_vecCl;
	std::vector<channel> _vecCh;

public:
	server(int port, std::string password);
	struct pollfd GetPollFd() const;
	void WaitForConnectServ();
	int	getIdSocket();
	void checkPollRevents(std::vector<struct pollfd> *vec);
	bool checkPassword(int fd);
	void returnPollClients(std::vector<struct pollfd> *vec);
	std::vector<client> &getVecCl();

	void Identification(int fd_client, std::vector<struct pollfd> *vec, client cl);

	//parse
	void parse(std::string message, client cl);
	void joinCmd(std::vector<std::string> content, client cl);
	void inviteCmd(std::string name, client cl);
	void kickCmd(std::string name, client cl);
	void topicCmd();
	void modeCmd(std::vector<std::string> cmd);
	void passCmd(client &cl);

	~server();
};

#endif