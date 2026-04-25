#include "../libs/main.hpp"

channel::channel(std::vector<std::string> content) : _nbAdmin(0), _private(false)
{
    if (content.size() > 2)
    {
        _key = content[2];
        _hasKey = true;
    }
    else
        _hasKey = false;
    _name = content[1];
}

channel::~channel() {}

std::string channel::getKey() const
{
    return _key;
}

void channel::sendToAll(client &cl, std::string message)
{
    //conditionnal jump
    int i = 0;
    std::string hex_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\r\n";
    /*std::string nc_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\n";*/
    std::cerr << hex_mess << std::endl;
    for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++)
	{
        if (_channelClients[i].getOut() != cl.getOut())
        {
            send(_channelClients[i].getOut(), hex_mess.c_str(), hex_mess.size(), 0);
        }
		i++;
	}
}

void channel::FrogSendToAll(std::string message)
{
    int i = 1;
    std::string hex_mess = ":Frogy!~BestFrogForEver@localhost PRIVMSG #channel :" +
                        message + "\r\n";
    /*std::string nc_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\n";*/
    std::cerr << hex_mess << std::endl;
    for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++)
	{
        send(_channelClients[i].getOut(), hex_mess.c_str(), hex_mess.size(), 0);
		i++;
	}
}

void channel::addNewClient(client cl) {
    if (isOnTheChannel(cl))
        return ;
    else
    {
        _channelClients.push_back(cl);
        if (_nbAdmin == 0)
            allowOperator(cl.GetNickname());
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

void channel::allowkey(std::vector<std::string> cmd, int out)
{
    if (_hasKey)
    {
        _hasKey = false;
        return;
    }
    else
    {
        if (cmd.size() > 3)
        {
            _key.clear();
            _key = cmd[3];
            _hasKey = true;
        }
        else
            send(out, "Error: You need to enter the password to set one\n", 50, 0);
    }
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
    return (find(_list.begin(), _list.end(), cl) != _list.end());
}

bool channel::isOnTheChannel(client cl)
{
    return (find(_channelClients.begin(), _channelClients.end(), cl) != _channelClients.end());
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

bool channel::validUser(std::string nick)
{
    size_t i = 0;
    while (i < _channelClients.size())
    {
        if (_channelClients[i].GetNickname() == nick)
            return true;
        i++;
    }
    return false;
}

void channel::allowOperator(std::string nick)
{
    size_t  j = 0;
    while (j < _admin.size())
    {
        if (_admin[j].GetNickname() == nick)
        {
            _admin.erase(_admin.begin() + j);
            --_nbAdmin;
            return;
        }
        j++;
    }
    size_t i = 0;
    while (i < _channelClients.size())
    {
        if (_channelClients[i].GetNickname() == nick)
            break;
        i++;
    }
    _admin.push_back(_channelClients[i]);
    ++_nbAdmin;
}