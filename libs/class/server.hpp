#ifndef SERVER_HPP
#define SERVER_HPP

#include "../main.hpp"
#include "../frogy/frogy.hpp"

class client;

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
	~server();
};

#endif