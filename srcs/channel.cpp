#include "../libs/main.hpp"

channel::channel(){}
channel::channel(std::string name, std::string key) : _name(name), _key(key), _private(false)
{}
channel::~channel() {}

//void channel::setTopic(std::string newTopic) {}
void channel::sendToAll() {}

void channel::addNewClient(client cl) {
/*peut-etre un petit checking pour savoir si le 
client est deja dans le vector ou pas*/
    _channelClients.push_back(cl);
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