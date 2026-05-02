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
    bool _hasLimit;
    bool _resTopic;
    size_t  _limitCl;
    frogy   _Frog;

public:
    channel(std::vector<std::string> content);
    channel(std::string _name);
    ~channel();

    std::string getKey() const;

    void sendToAll(client &cl, std::vector<std::string> &content);
    void FrogSendToAll(std::string message);
    void addNewClient(client &cl);
    void addOnList(client cl);

    std::string getname();
    void kick(client cl);

    void allowInvite();
    void setInviteOnly();
    void unsetInviteOnly();

    void allowkey(std::string pass);
    void UnsetKey(std::vector<std::string> cmd);
    void setKey(std::vector<std::string> cmd);

    void setTopic(std::string cmd);
    std::string getTopic();
    void setResTopic();
    void unsetResTopic();

    void setLimitCl(size_t limit);
    bool hasLimit() const;
    size_t getLimitCl();
    void UnsetLimitCl();

    bool sameName(std::string str);
    bool isPrivate() const;
    bool hasKey() const;
    bool isOnTheList(client cl);
    bool isOnTheChannel(client cl);
    bool isAdmin(client cl);
    std::vector<client> &getchannelClients();
    bool getResTopic() const;
    void allowResTopic();
    bool validUser(std::string name);

    void unallowOperator(std::vector<std::string> cmd);
    void allowOperator(std::vector<std::string> cmd);
    size_t size();
};