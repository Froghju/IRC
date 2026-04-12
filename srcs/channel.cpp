#include "../libs/class/channel.hpp"
#include "../libs/class/client.hpp"
#include "../libs/main.hpp"

channel::channel(std::string name, std::string key) : _name(name), _key(key), _private(false)
{}

channel::~channel() {}

void channel::setTopic() {}
void channel::sendToAll() {}
bool channel::sameName(std::string str) {
    if (str == _name)
        return true;
    return false;
}
bool channel::isPrivate()
{
    return private;
}