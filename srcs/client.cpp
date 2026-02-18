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
    }
}

client::~client() {}

sockaddr_in client::GetClientInfo()
{
	return (_clientInfo);
}

void client::ClientConnect(server &svr) {
    int error = connect(_clientId, (struct sockadd*)&svr.GetServInfo());
    if (error != 0)
        throw InvalidClientSig("Invalid Client connection to Server");
}

void client::ClientSend(server &svr) {
    
    _clientFd = svr.ConnectServ(*this);
    if (_clientFd == -1)
        throw InvalidClientSig("Client connection failed: invalid Fd");
    //Message Msg; //comment remplir le buff ? std::cin ?
    //Msg.size = strlen(Msg.buff);
    //long int size = send(_clientId, Msg.buff, Msg.size, 0);

    //COMENT ON RECUP CE FUCKING MESSAGE ????

    if (Mgs.size == -1)
        throw InvalidClientSig("Inable to send from client");

    //Appeler la fonction recv() du serveur
}

void client::ClientRecv(server &svr) {

    // big question A CHERCHER DEMAIN
    WaitForReponseServ();
    if (Msg.size == -1)
        throw InvalidClientSig("Client reception error");
}

void WaitForReponseServ() {
    //meme chose que le server, attendre la reponse
}