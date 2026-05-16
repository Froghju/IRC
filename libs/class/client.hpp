#pragma once
#include "server.hpp"
class server;

struct ready {
    bool pass;
    bool user;
    bool nick;
    bool all;
};

class client {

private:
    client();

    int _clientId;
    int _out;
    sockaddr_in _clientInfo;

    std::string _UserName;
    std::string _Nickname;
    bool _Operator;
    bool _Hex;
    bool _admin;
    bool _inChannel;
    std::string _buffMessage;
    struct ready _isReady;

public:
    client(int port);
    client &operator=(const client & src);
    ~client();

	sockaddr_in &SetClientInfo();
    void setClientName(std::string str);
    void setNickname(std::string str);
    void setOperator(bool perm);
    void setHex(bool b);
    void setOut(int c);
    void setFdOut(int out);
    void setReady(char c);

    int getOut() const;
    bool getHex() const;
	std::string GetClientUserName() const;
    std::string GetNickname() const;
    sockaddr_in GetClientInfo() const;
    int GetClientID() const;
    bool GetOperator() const;
    bool GetReady() const;
    bool GetPass() const;

    struct pollfd InitPollFd(int fd);
    bool checkPollRevents(std::vector<struct pollfd> *vec, int i, server &serv);
    bool getInChannel();
    std::string conCat(const char *buff);
    void resetMess(std::string str);

    bool operator==(const client &src) const;
    bool operator!=(const client &src) const;

    class InvalidClientSig : public std::exception {
        public:
            const char *_error;

            InvalidClientSig(const char *data) : _error(data) {}
            virtual const char* what() const throw()
            {
                return (_error);
            }
    };
};