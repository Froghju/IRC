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
    int i = 0;
    int j = 0;
    while (i <= _channelClient[i].size())
    {
        if (_channelClient[i] == cl)
            _channelClient.erase(i);
        i++;
    }
    while (j <= _list[j].size())
    {
        if (_list[j] == cl)
            _list.erase(j);
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
bool channel::isPrivate()
{
    return _private;
}

bool channel::isOnTheList(client cl) const
{
    size_t i = 0;
    while (_list[i] != cl)
        i++;
    if (i > vec[i].size())
        return false;
    else
        return true;
}