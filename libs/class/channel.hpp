#pragma once
#include "client.hpp"
#include "../main.hpp"

class client;
class server;

class channel
{
private:
    channel();
    std::string _name;
    std::string _key;
    std::string _topic;
    std::vector<client> _channelClients;
    std::vector<client> _list;
    std::vector<client> _admin;
    int _nbAdmin;
    bool _private;
    bool _hasKey;
    bool _resTopic;
    size_t  _limitCl;

public:
    channel(std::vector<std::string> content);
    channel(std::string _name);
    ~channel();

    std::string getKey() const;

    void sendToAll(client &cl, std::string message, server &serv);
    void addNewClient(client cl);
    void addOnList(client cl);

    void kick(client cl);
    void allowInvite();
    void allowkey(std::vector<std::string> cmd, int out);
    void setTopic(std::string cmd);
    std::string getTopic();
    void setLimitCl(size_t limit);
    size_t getLimitCl();

    bool sameName(std::string str);
    bool isPrivate() const;
    bool hasKey() const;
    bool isOnTheList(client cl);
    bool isOnTheChannel(client cl);
    bool isAdmin(client cl);
    std::vector<client> getchannelClients();
    bool getResTopic();
    void allowResTopic();
    bool validUser(std::string name);
    void allowOperator(std::string name);
};