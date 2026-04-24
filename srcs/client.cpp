#include "../libs/class/client.hpp"
#include "../libs/main.hpp"

client::client() {}

client::client(int port) {
    _clientId = socket(AF_INET, SOCK_STREAM, 0);

    if (_clientId == -1)
        throw InvalidClientSig("Invalid Client Socket");
    else
    {
        _clientInfo.sin_family = AF_INET;
        _clientInfo.sin_port = htons(port);
        _clientInfo.sin_addr.s_addr = INADDR_ANY;
        //_size = socklen_t(sizeof(_clientInfo));
        _out = -1;
        _inChannel = false;
    }
}

client &client::operator=(const client & src)
{
    _clientId = src._clientId;
    _clientInfo = src._clientInfo;
    //_size = src._size;
    _Hex = src._Hex;
    return(*this);
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
    //delete _size;
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

/*socklen_t client::GetClientSize() const
{
    return (_size);
}*/

bool client::checkPollRevents(struct pollfd pipoll, server &serv)
{
    if (pipoll.events != 0)
    {
        if (pipoll.revents & POLLIN)
        {
			std::string all_text = read_mess(pipoll.fd);
            //std::cerr << "ICI: " << all_text << std::endl;
            if (!all_text.empty())
            {
                serv.ExecCmd(*this, all_text);
            }
            else
            {
                std::cout << _UserName << " quit serv" << std::endl;
                return false;
            }
        }
        if (pipoll.revents & POLLHUP)
        {
            std::cerr << "erreur pollhup" << std::endl;
            return false;
        }
        if (pipoll.revents & POLLERR)
        {
            std::cerr << "erreur pollerr" << std::endl;
            return false;
        }
        pipoll.revents = 0;
    }
    return true;
}

void client::setClientName(std::string str)
{
    //std::cerr << "str : "<<str << std::endl;
    _UserName = str; 
    //std::cerr << "_UserName : " << _UserName << std::endl;
}

std::string client::GetClientUserName() const
{
    //std::cerr << _UserName << std::endl;
    return (_UserName);
}

void client::setNickname(std::string str)
{
    //std::cerr << "str : " << str<< std::endl;  
    _Nickname = str;
    //std::cerr << "_Nickname : " << _Nickname << std::endl;  
}

std::string client::GetNickname() const
{
    //std::cerr << _Nickname << std::endl;
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

int client::getOut() const //ATTENTION 2 FOIS LES MEME
{
    return(_out);
}

int client::GetFdOut() const //ATTENTION 2 FOIS LES MEME
{
    return _out;
}

bool client::operator==(const client &src) const
{
    if (_clientId == src._clientId
        && _out == src._out
        && _clientInfo.sin_addr.s_addr == src._clientInfo.sin_addr.s_addr
        && _clientInfo.sin_port == src._clientInfo.sin_port
        //&& _size == src._size
        && _UserName == src._UserName
        && _Nickname == src._Nickname
        && _Operator == src._Operator)
        return true;
    else
        return false;
}

bool client::operator!=(const client &src) const
{
    std::cerr << _out << std::endl;
    std::cerr << _clientInfo.sin_addr.s_addr << std::endl;
    std::cerr << _clientInfo.sin_port << std::endl;
    std::cerr << _clientId << std::endl;
    if (_clientId != src._clientId
        && _out != src._out
        && _clientInfo.sin_addr.s_addr != src._clientInfo.sin_addr.s_addr
        && _clientInfo.sin_port != src._clientInfo.sin_port
        //&& _size != src._size
        && _UserName != src._UserName
        && _Nickname != src._Nickname
        && _Operator != src._Operator)
        return true;
    else
        return false;
}

bool client::getInChannel()
{
    return _inChannel;
}