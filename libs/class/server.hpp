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
	size_t findChannel(std::string name);
	bool validUser(std::string name);
	void inviteCmd(std::vector<std::string> content, client admin);
	void kickCmd(std::vector<std::string> content, client admin);
	void modeCmd(std::vector<std::string> cmd, client cl);
	void passCmd(std::string cmd, client &cl);
	client &findClient(std::string clientNick);
	std::vector<std::string> findAllOptCmd(std::string mess, std::string cmd);
	void ExecCmd(std::vector<struct pollfd> *vec, client &cl, std::string mess);
	void joinCmd(std::string channelName, std::string clientName, client cl);
	void topicCmd(std::vector<std::string> cmd, client &cl);
	void sendToClient(std::vector<std::string> mess);
	bool initClient(client &cl);

	class ChannelNotFound : public std::exception {

        public:

            virtual const char* what() const throw()
            {
                return ("Channel name not found");
            }
    };

	~server();
};

#endif