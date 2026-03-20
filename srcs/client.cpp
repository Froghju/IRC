#include "../libs/class/client.hpp"
#include "../libs/main.hpp"
//#include "../libs/class/string.hpp"

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

void client::checkPollRevents(struct pollfd pipoll)
{
    if (pipoll.events != 0)
    {
        if (pipoll.revents & POLLHUP)
        {
            std::cout << "erreur pollhup" << std::endl;
        }
        if (pipoll.revents & POLLERR)
        {
            std::cout << "erreur pollerr" << std::endl;
        }
        if (pipoll.revents & POLLIN)
        {
            string test;
            int nb = 0;
            while (nb <= 0)
            {
                char buff[10];
                nb = read(pipoll.fd, buff, 9);
                if (nb == -1)
                {
                    std::cout << "erreur" << std::endl;
                    break;
                }
                else
                {
                    buff[nb] = '\0';
                    test += buff;
                }
            }
            if (nb != -1)
            {
                //std::cout << test;
                recv(_clientId, test.str(), test.size(), 0);
                //std::cout << "Message from client: " << test << std::endl;
            }
        }
        pipoll.revents = 0;
    }
}

void client::setClientName(std::string str)
{
    _UserName = str;
    std::cout << _UserName << std::endl;
}

string client::GetClientUserName()
{
    return (_UserName);
}