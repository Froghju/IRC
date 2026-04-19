#pragma once
#include "server.hpp"
class server;

class client {

private:
    client();

    int _clientId;
    int _out;
    sockaddr_in _clientInfo;
    socklen_t * _size;

    std::string _UserName;
    std::string _Nickname;
    bool _Operator;
    bool _Hex;
    int _step;

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
    int getOut() const;
    bool getHex() const;    void setFdOut(int out);
    void addStep();

	std::string GetClientUserName() const;
    std::string GetNickname() const;
    sockaddr_in GetClientInfo() const;
    socklen_t * GetClientSize() const;
    bool GetOperator() const;
    int GetFdOut() const;
    int GetStep() const;

    struct pollfd InitPollFd(int fd);
    bool checkPollRevents(struct pollfd pipoll, std::vector<struct pollfd> *vec, server &serv);
    void doCmd(std::string msg, server *serv);

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