#pragma once
#include "client.hpp"

class client;

class channel
{
private:
    channel();
    std::string _name;
    std::string _key;
    std::string _topic;
    std::vector<client> _channelClients;
    std::vector<client> _list;

    bool _private;
    bool _inviteOnly;

public:
    channel(std::string _name, std::string key);
    ~channel();

    //void setTopic(std::string newTopic);
    void sendToAll(); // not done
    void addNewClient(client cl);
    void addOnList(client cl);
    void kick(client cl);
    void allowInvate();

    bool sameName(std::string str);
    bool isPrivite() const;
    bool isOnTheList(client cl) const;
};