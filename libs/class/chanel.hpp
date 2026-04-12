#pragma once
#include "client.hpp"

class client;

class channel
{
private:
    std::string _name;
    std::string _key;
    std::string _topic;
    std::vector<client> _channelClients;
    bool _private;

public:
    channel(std::string _name, key);
    ~channel();

    void setTopic(std::string newTopic);
    void sendToAll();
    void addNewClient(client cl);
    bool sameName(std::string str);
    bool isPrivate();
}