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
    }
}

struct pollfd client::InitPollFd(int fd)
{
    struct pollfd cpfd;
    cpfd.fd = fd;
    cpfd.events = 1;
    cpfd.revents = 0;
    return cpfd;
}

client::~client() {
    close(_clientId);
    if (_size)
        delete _size;
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

    *_size = sizeof(_clientInfo);
    return (_size);
}