#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.hpp"
#include "../frogy/frogy.hpp"

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
	frogy	_Fro;
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

	bool Identification(std::vector<struct pollfd> *vec, client &cl);
	bool initUserNick(client &cl);
	bool initHex(client &cl);
	bool initNetcat(client &cl);
	//parse
	void parse(std::string message, client cl);
	void joinCmd(std::vector<std::string> content, client cl);
	void topicCmd();
	size_t findChannel(std::string name);
	bool validUser(std::string name);
	void inviteCmd(std::string channel, client admin, client cl);
	void kickCmd(std::string name, client admin, client cl);
	void modeCmd(std::vector<std::string> cmd, client cl);
	void passCmd(std::string cmd, client &cl);
	bool isChannel(std::string name);

	~server();
};

#endif