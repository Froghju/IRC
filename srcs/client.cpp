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
        _size = new(socklen_t);
        *_size = sizeof(_clientInfo);
    }
}

client &client::operator=(const client & src)
{
    _clientId = src._clientId;
    _clientInfo = src._clientInfo;
    _size = src._size;
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

socklen_t * client::GetClientSize() const
{
    return (_size);
}

bool client::checkPollRevents(struct pollfd pipoll, std::vector<struct pollfd> *vec, server *serv)
{
    if (pipoll.events != 0)
    {
        if (pipoll.revents & POLLIN)
        {
			std::string all_text = read_mess(pipoll.fd);
            if (!all_text.empty())
            {
                server tmp = (*serv);
                /*doCmd(all_text, serv);*/
                sendToAll(pipoll.fd, vec, all_text, _Nickname);
            }
            else
            {
                std::cout << _UserName << " quit serv" << std::endl;
                return false;
            }
        }
        if (pipoll.revents & POLLHUP)
        {
            std::cout << "erreur pollhup" << std::endl;
            return false;
        }
        if (pipoll.revents & POLLERR)
        {
            std::cout << "erreur pollerr" << std::endl;
            return false;
        }
        pipoll.revents = 0;
    }
    return true;
}

/*void client::doCmd(std::string msg, server *serv)
{
    //fct start commande
}*/

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
    _Nickname = str;
}

std::string client::GetNickname() const
{
    return (_Nickname);
}

void client::setOperator(bool perm)
{
    _Operator = perm;
}

bool client::GetOperator() const
{
    return (_Operator);
}