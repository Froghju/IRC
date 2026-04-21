#include "../libs/main.hpp"

channel::channel() : _nbAdmin(0), _private(false) {}
channel::channel(std::string name, std::string key) : _name(name), _key(key), _nbAdmin(0), _private(false) {}
channel::~channel() {}

std::string channel::getKey() const
{
    return _key;
}

//void channel::setTopic(std::string newTopic) {}
void channel::sendToAll(client &cl, std::string message, server &serv)
{
    int i = 1;
    std::string hex_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\r\n";
    std::string nc_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\n";
    std::cerr << hex_mess << std::endl;
    for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++)
	{
        if (_channelClients[i].getOut() != cl.getOut())
        {
            if (serv.getVecCl()[i-1].getHex())
                send(_channelClients[i].getOut(), hex_mess.c_str(), hex_mess.size(), 0);
            else
                send(_channelClients[i].getOut(), nc_mess.c_str(), nc_mess.size(), 0);
        }
		i++;
	}
}

void channel::addNewClient(client cl) {
    if (isOnTheChannel(cl))
        return ;
    else
    {
        if (_nbAdmin == 0)
            //METTRE EN ADMIN (fonction comme mode -o)
        _channelClients.push_back(cl);
    }
}

void channel::addOnList(client cl) 
{
    if (!isOnTheList(cl))
        _list.push_back(cl);
}

void channel::kick(client cl)
{
    size_t i = 0;
    size_t j = 0;
    while (i <= _channelClients.size())
    {
        if (cl.GetOperator())
        {
            if (_nbAdmin == 1)
            {
                send(cl.GetFdOut(), "Invalid command: An operator must be in the channel\n", 53, 0);
                return ;
            }
            if (_channelClients[i] == cl)
                _channelClients.erase(_channelClients.begin() + i);
            i++;
        }
    }
    while (j <= _list.size())
    {
        if (_list[j] == cl)
            _list.erase(_list.begin() + j);
        j++;
    }
}

void channel::allowInvite()
{
    if (_private)
    {
        _private = false;
        return;
    }
    else
        _private = true;
}



bool channel::sameName(std::string str) {
    if (str == _name)
        return true;
    return false;
}
bool channel::isPrivate() const
{
    return _private;
}

bool channel::isOnTheList(client cl)
{
    size_t i = 0;
    while (_list[i] != cl)
        i++;
    if (i > _list.size())
        return false;
    else
        return true;
}

bool channel::isOnTheChannel(client cl)
{
    size_t i = 0;
    while (_channelClients[i] != cl)
        i++;
    if (i > _channelClients.size())
        return false;
    else
        return true;
}