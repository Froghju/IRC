#include "../libs/class/client.hpp"
#include "../libs/main.hpp"

client::client() {}

client::client(int port) : _Operator(false) {
    _clientId = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(_clientId, F_SETFL, O_NONBLOCK);

    if (_clientId == -1)
        throw InvalidClientSig("Invalid Client Socket");
    else
    {
        _clientInfo.sin_family = AF_INET;
        _clientInfo.sin_port = htons(port);
        _clientInfo.sin_addr.s_addr = INADDR_ANY;
        _out = -1;
        _inChannel = false;
        _isReady.pass = false;
        _isReady.user = false;
        _isReady.nick = false;
        _isReady.all = false;
    }
}

client &client::operator=(const client & src)
{
    if (this == &src)
        return *this;
    _clientId = src._clientId;
    _clientInfo = src._clientInfo;
    _Hex = src._Hex;
    _out = src._out;
    _UserName = src._UserName;
    _Nickname = src._Nickname;
    _Operator = src._Operator;
    _admin = src._admin;
    _inChannel = src._inChannel;
    _buffMessage = src._buffMessage;
    _isReady = src._isReady;
    return *this;
}

struct pollfd client::InitPollFd(int fd)
{
    struct pollfd cpfd;
    cpfd.fd = fd;
    cpfd.events = POLLIN;
    cpfd.revents = 0;
    return cpfd;
}

client::~client() {
    shutdown(_clientId, SHUT_RDWR);
    close(_clientId);
}

sockaddr_in &client::SetClientInfo()
{
	return (_clientInfo);
}

sockaddr_in client::GetClientInfo() const
{
	return (_clientInfo);
}

int client::GetClientID() const
{
    return (_clientId);
}

bool client::checkPollRevents(std::vector<struct pollfd> *vec, int i, server &serv)
{
    if ((*vec)[i].events != 0)
    {
        if ((*vec)[i].revents & POLLIN)
        {
            try
            {
                std::string all_text = read_mess(*this);
                if (!all_text.empty())
                {
                    if (this->GetReady())
                        serv.ExecCmd(*this, all_text);
                    else
                        serv.Identification(*this, all_text);
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                serv.eraseClient(*this);
                (*vec).erase((*vec).begin() + i);
            }
        }
        if ((*vec)[i].revents & POLLHUP)
        {
            std::cerr << "erreur pollhup" << std::endl;
            return false;
        }
        if ((*vec)[i].revents & POLLERR)
        {
            std::cerr << "erreur pollerr" << std::endl;
            return false;
        }
        (*vec)[i].revents = 0;
    }
    return true;
}

void client::setClientName(std::string str)
{
    _UserName = str; 
}

std::string client::GetClientUserName() const
{
    return (_UserName);
}

void client::setNickname(std::string str)
{
    std::cout << "Nickname =" << _Nickname << std::endl;
    _Nickname = str;
    std::cout << "Nickname =" << _Nickname << std::endl;
}

std::string client::GetNickname() const
{
    return (_Nickname);
}

void client::setOperator(bool perm)
{
    _Operator = perm;
}

void client::setFdOut(int out) 
{
    _out = out;
}

bool client::GetOperator() const
{
    return (_Operator);
}

void client::setHex(bool b)
{
    _Hex = b;
}

bool client::getHex() const
{
    return(_Hex);
}

void client::setOut(int out)
{
    _out = out;
}

int client::getOut() const
{
    return(_out);
}

bool client::operator==(const client &src) const
{
    if (_clientId == src._clientId
        && _Nickname == src._Nickname)
        return true;
    else
        return false;
}

bool client::operator!=(const client &src) const
{
    if (_clientId != src._clientId
        || _Nickname != src._Nickname)
        return true;
    else
        return false;
}

bool client::getInChannel()
{
    return _inChannel;
}

std::string client::conCat(const char *buff)
{
    _buffMessage += buff;
    return _buffMessage;
}

void client::resetMess(std::string str)
{
    _buffMessage.clear();
    _buffMessage += str;
}

void client::setReady(char c)
{
    if (c == 'P')
        _isReady.pass = true;
    else if (c == 'U')
        _isReady.user = true;
    else if (c == 'N')
        _isReady.nick = true;
    if (_isReady.pass && _isReady.user && _isReady.nick)
        _isReady.all = true;
}

bool client::GetReady() const 
{
    return _isReady.all;
}

bool client::GetPass() const 
{
    return _isReady.pass;
}