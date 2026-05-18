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
	std::string _ServName;
	std::string _PassW;
	int _Port;
	int	_IdSocket;

	sockaddr_in	_InfServ;
	struct pollfd _vpfd;
	std::vector<client>	_vecCl;
	std::vector<channel> _vecCh;
	frogy	_Fro;

public:
	server(int port, std::string password);
	struct pollfd GetPollFd() const;
	void WaitForConnectServ();
	int	getIdSocket() const;
	void checkPollRevents(std::vector<struct pollfd> *vec);
	void returnPollClients(std::vector<struct pollfd> *vec);
	std::vector<client> &getVecCl();

	void Identification(client &cl, std::string all_text);
	bool isvalidNickname(std::string input, client &cl);
	bool isvalidUsername(std::string input, client &cl);
	void deleteClient(client &cl);

	//parse
	void joinCmd(std::vector<std::string> content, client &cl);
	size_t findChannel(std::string name);
	bool validUser(std::string name);
	void inviteCmd(std::vector<std::string> content, client &admin);
	void kickCmd(std::vector<std::string> content, client admin);
	void modeCmd(std::vector<std::string> cmd, client cl);
	client &findClient(std::string clientNick);
	void ExecCmd(client &cl, std::string mess);
	void topicCmd(std::vector<std::string> cmd, client &cl);
	void sendToClient(std::vector<std::string> mess, client &cl);
	void eraseClient(client &cl);
	std::string usernamehexchat(std::string &input);

	void	sendjoin(size_t pos, client &cl);
	void	sendlistclchannel(size_t pos);
	void	sendoperator(size_t pos, client &cl);
	void	sendNoTopic(size_t pos, client &cl);
	void	sendTopic(size_t pos, client &cl);
	void	sendNoTopicAll(size_t pos);
	void	sendTopicAll(size_t pos);
	void	sendInvite(client &cl, size_t pos);
	void	sendInviteOnly(client &cl, size_t pos);
	void	unsendoperator(size_t pos, client &cl);
	void	sendNewNick(client &cl, std::string newname);
	void	sendlistclallchannel(client &cl);
	void	sendlistclchannel(channel &chan);
	void	eraseClientChannel(client &cl);
	void	sendlistclall();
	class ChannelNotFound : public std::exception {

        public:

            virtual const char* what() const throw()
            {
                return ("Channel name not found");
            }
    };
		class ClientNotFound : public std::exception {

        public:

            virtual const char* what() const throw()
            {
                return ("Client not found");
            }
    };
	class NoMessage : public std::exception {

        public:
		
            virtual const char* what() const throw()
            {
                return ("No message");
            }
    };
	~server();
};

#endif