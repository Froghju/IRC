#include "../libs/class/client.hpp"
#include <netinet/in.h> // for sockaddr_in struct
#include <sys/socket.h>

client::client() {}

client::client(int port) {
    _clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_clientSocket == -1)
        throw /*INVALID*/;
    else
    {
        _clientInfo.sin_family = AF_INET;
        _clientInfo.sin_port = htons(port);
        _clientInfo.sin_addr.s_addr = INADDR_ANY;

        //setsockopt() ??
    }
}

sockaddr_in client::getClientInfo()
{
	return (_clientInfo);
}

client::~client() {}