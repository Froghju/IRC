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
    bool _hasKey;

public:
    channel(std::string _name, std::string key);
    channel(std::string _name);
    ~channel();

    std::string getKey() const;

    void sendToAll(client &cl, std::string message, server &serv);
    void addNewClient(client cl);
    void addOnList(client cl);

    void kick(client cl);
    void allowInvite();
    void allowkey(std::vector<std::string> cmd);

    bool sameName(std::string str);
    bool isPrivate() const;
    bool hasKey() const;
    bool isOnTheList(client cl);
    bool isOnTheChannel(client cl);
};