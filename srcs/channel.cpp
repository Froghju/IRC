#include "../libs/main.hpp"

channel::channel(std::vector<std::string> content) : _nbAdmin(0), _private(false), _hasLimit(false), _resTopic(true)
{
    if (content.size() > 2)
    {
        _key = content[2];
        _hasKey = true;
    }
    else
        _hasKey = false;
    _name = content[1];
    std::cerr << "Channel '" << _name << "' created" << std::endl;
}

channel::~channel() {}

std::string channel::getKey() const
{
    return _key;
}

void channel::sendToAll(client &cl, std::vector<std::string> &content)
{
    int i = 0;
    std::string message;
    for (std::vector<std::string>::iterator it = content.begin() + 2; it != content.end(); ++it)
    {
        message += *it;
        if (it + 1 != content.end())
            message += " ";
        else
            message += "\r\n";
    }
    std::cerr << "message = " << message << std::endl;
    std::string hex_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost " + content[0] + " " + content[1] + " " +
                        message;
    std::cerr << "hex_mess = " << hex_mess << std::endl;
    for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++)
	{
        std::cerr << "client nickname send mess = " << _channelClients[i].GetNickname() << std::endl;
        if (_channelClients[i].getOut() != cl.getOut())
        {
            send(_channelClients[i].getOut(), hex_mess.c_str(), hex_mess.size(), 0);
        }
		i++;
	}
}

void channel::FrogSendToAll(std::string message, std::vector<std::string> &content)
{
    int i = 0;
    std::string hex_mess = ":Frogy!~BestFrogForEver@localhost PRIVMSG " + content[1] + " :" +
                        message + "\r\n";
    for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); it++)
	{
        send(_channelClients[i].getOut(), hex_mess.c_str(), hex_mess.size(), 0);
		i++;
	}
}

void channel::addNewClient(client &cl) {
    if (isOnTheChannel(cl))
        return ;
    else
    {
        if (_nbAdmin == 0)
        {
            cl.setOperator(true);
            _admin.push_back(cl);
            ++_nbAdmin;
        }
        std::cerr << "add client channel = " << &cl << std::endl;
        _channelClients.push_back(cl);
        std::cerr << "add client channel push = " << &(--_channelClients.end()) << std::endl;
    }
}

void channel::addOnList(client cl) 
{
    if (!isOnTheList(cl))
        _list.push_back(cl);
}

void channel::kick(client cl)
{
    if (_nbAdmin == 1 && cl.GetOperator())
    {
        std::cerr << cl.GetNickname() << std::endl;
        send(cl.getOut(), "Invalid command: An operator must be in the channel\n", 53, 0);
        return ;
    }

    std::vector<client>::iterator it = std::find(_channelClients.begin(), _channelClients.end(), cl);
    if (it != _channelClients.end())
        _channelClients.erase(it);
    std::vector<client>::iterator itt = std::find(_list.begin(), _list.end(), cl);
    if (itt != _list.end())
        _list.erase(itt);
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

void channel::setInviteOnly()
{
    _private = true;
}

void channel::unsetInviteOnly()
{
    _private = false;
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

void channel::allowkey(std::string pass)
{
    _key.clear();
    _key = pass;
    _hasKey = true; 
}

void channel::UnsetKey(std::vector<std::string> cmd)
{
    if (!_hasKey)
        return;
    if (cmd.size() != 4)
    {
        return;
    }
    if (cmd[3] != _key)
    {
        return;
    }
    _hasKey = false;
}

void channel::setKey(std::vector<std::string> cmd)
{
    std::cerr << "check" << std::endl;
    if (cmd.size() != 4)
    {
        std::cerr << "check 2" << std::endl;
        return;
    }
    std::cerr << "check 3" << std::endl;
    if (!_key.empty())
    {
        std::cerr << "check 4" << std::endl;
        _key.clear();
    }
    std::cerr << "check 5" << std::endl;
    _key = cmd[3];
    _hasKey = true;
    std::cerr << "check 6" << std::endl;
}

bool channel::isOnTheList(client &cl)
{
    return (find(_list.begin(), _list.end(), cl) != _list.end());
}

bool channel::isOnTheChannel(client &cl)
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

bool channel::isAdmin(client &cl)
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
    _hasLimit = true;
}

void channel::UnsetLimitCl()
{
    _hasLimit = false;
}

bool channel::hasLimit() const
{
    return _hasLimit;
}

size_t channel::getLimitCl()
{
    return (_limitCl);
}

std::vector<client> &channel::getchannelClients()
{
    return (_channelClients);
}

std::vector<client> &channel::getchannelAdmin()
{
    return (_admin);
}

bool channel::getResTopic() const
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

void channel::setResTopic()
{
    _resTopic = true;
}

void channel::unsetResTopic()
{
    _resTopic = false;
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

void channel::allowOperator(std::vector<std::string> cmd)
{
    for (size_t i = 0; i < _channelClients.size(); i++)
    {
        if (_channelClients[i].GetNickname() == cmd[3])
        {
            if (!isAdmin(_channelClients[i]))
            {
                _admin.push_back(_channelClients[i]);
                sendoperator(_channelClients[i]);
                ++_nbAdmin;
            }
        }
    }
}

void channel::unallowOperator(std::vector<std::string> cmd)
{
    for (size_t i = 0; i < _channelClients.size(); i++)
    {
        if (_channelClients[i].GetNickname() == cmd[3])
        {
            if (isAdmin(_channelClients[i]) && _nbAdmin > 1)
            {
                std::vector<client>::iterator it = std::find(_admin.begin(), _admin.end(), _channelClients[i]);
                _admin.erase(it);
                unsendoperator(_channelClients[i]);
                --_nbAdmin;
            }
        }
    }
}

size_t channel::size()
{
    return _channelClients.size();
}

std::string channel::getname()
{
    return (_name);
}