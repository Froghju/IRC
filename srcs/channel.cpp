#include "../libs/main.hpp"

channel::channel(std::string name, std::string key) : _name(name), _key(key), _nbAdmin(0), _private(false), _hasKey(true) {}
channel::channel(std::string name) : _name(name), _nbAdmin(0), _private(false), _hasKey(false) {}
channel::~channel() {}

std::string channel::getKey() const
{
    return _key;
}

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

void channel::allowkey()
{
    if (_hasKey)
    {
        _hasKey = false;
        return;
    }
    else
        _hasKey = true;
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

bool channel::hasKey() const
{
    return _hasKey;
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

void channel::setTopic(std::string topic)
{
    if (!_topic.empty())
        _topic.clear();
    _topic = topic;
}


std::string channel::getTopic()
{
    return (_topic);
}

bool channel::isAdmin(client cl)
{
    for (size_t i = 0; i < _admin.size(); i++)
    {
        if (cl == _admin[i])
            return true;
    }
    return false;
}

void channel::setLimitCl(size_t limit)
{
    _limitCl = limit;
}

size_t channel::getLimitCl()
{
    return (_limitCl);
}

std::vector<client> channel::getchannelClients()
{
    return (_channelClients);
}

bool channel::getResTopic()
{
    return (_resTopic);
}

void channel::allowResTopic()
{
    if (_resTopic)
        _resTopic = false;
    else
        _resTopic = true;
}

bool channel::validUser(std::string name)
{
    size_t i = 0;
    while (i < _channelClients.size())
    {
        if (_channelClients[i].GetClientUserName() == name)
            return true;
        i++;
    }
    return false;
}

void channel::allowOperator(std::string name)
{
    size_t  j = 0;
    while (j < _admin.size())
    {
        if (_admin[j].GetClientUserName() == name)
        {
            _admin.erase(_admin.begin() + j);
            return;
        }
        j++;
    }
    size_t i = 0;
    while (i < _channelClients.size())
    {
        if (_channelClients[i].GetClientUserName() == name)
            break;
        i++;
    }
    _admin.push_back(_channelClients[i]);
}