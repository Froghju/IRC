#include "../libs/main.hpp"

channel::channel() : _private(false), _inviteOnly(false) {}
channel::channel(std::string name, std::string key) : _name(name), _key(key)
{
    _private = false;
    _inviteOnly = false;
}
channel::~channel() {}

//void channel::setTopic(std::string newTopic) {}
void channel::sendToAll() {}
void channel::addNewClient(client cl) {
/*peut-etre un petit checking pour savoir si le 
client est deja dans le vector ou pas*/
    _channelClients.push_back(cl);
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
        if (_channelClients[i] == cl)
            _channelClients.erase(_channelClients.begin() + i);
        i++;
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
    if (_inviteOnly)
    {
        _inviteOnly = false;
        return;
    }
    else
        _inviteOnly = true;
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

bool channel::isOnTheList(client cl) const
{
    size_t i = 0;
    while (i < _list.size())
    {
        if (_list[i] == cl)
            return true;
        i++;
    }
    return false;
}