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
    int _nbAdmin;

    bool _private;

public:
    channel(std::string _name, std::string key);
    ~channel();

    std::string getKey() const;

    //void setTopic(std::string newTopic);
    void sendToAll(); // not done
    void addNewClient(client cl);
    void addOnList(client cl);
    void kick(client cl);
    void allowInvite();

    bool sameName(std::string str);
    bool isPrivate() const;
    bool isOnTheList(client cl);
    bool isOnTheChannel(client cl);
};