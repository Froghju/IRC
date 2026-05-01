#include "../libs/class/server.hpp"
#include "../libs/class/channel.hpp"
#include "../libs/main.hpp"

size_t server::findChannel(std::string name)
{
    size_t i = 0;
    while (i < _vecCh.size())
    {
        if (_vecCh[i].sameName(name))
            return i;
        i++;
    }
    throw ChannelNotFound();
}

bool server::validUser(std::string name)
{
    size_t i = 0;
    while (i < _vecCl.size())
    {
        if (_vecCl[i].GetClientUserName() == name)
            return true;
        i++;
    }
    return false;
}

void server::joinCmd(std::vector<std::string> content, client &cl)
{
    if (content.size() > 1)
    {
        try
        {
            size_t i = findChannel(content[1]);
            if (_vecCh[i].hasLimit())
            {
                if (_vecCh[i].getLimitCl() == _vecCh[i].size())
                {
                    std::string ms = ":" + _ServName + " 471 :Channel is full\r\n";
					send(cl.getOut(), ms.c_str(), ms.size(), 0);
                    return ;
                }
            }
            if (_vecCh[i].hasKey())
            {
                if (content.size() > 2)
                {
                    if (content[2] != _vecCh[i].getKey())
                    {
                        std::string ms = ":" + _ServName + " 475 :Bad Channel key\r\n";
					    send(cl.getOut(), ms.c_str(), ms.size(), 0);
                        return ;
                    }
                }
                else
                {
					std::string ms = ":" + _ServName + " 461 :Need more params\r\n";
					send(cl.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            if (_vecCh[i].isPrivate())
            {
                if (_vecCh[i].isOnTheList(cl))
                    _vecCh[i].addNewClient(cl);
                else
                {
                    std::string ms = ":" + _ServName + " 473 :Invite only chan\r\n";
					send(cl.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            else
            {
                _vecCh[i].addNewClient(cl);
                return ;
            }
        }
        catch(const std::exception& e)
        {
            //message a envoyer comme quoi ok
            channel newchannel(content);
            newchannel.addNewClient(cl);
            _vecCh.push_back(newchannel);
        }
    }
    else
    {
        std::string ms = ":" + _ServName + " 461 :Params not ok\n";
        send(cl.getOut(), ms.c_str(), ms.size(), 0);
    }
}

void server::inviteCmd(std::vector<std::string> content, client &admin)
{
    if (content.size() > 2)
    {
        int subject = 0;
        try
        {
            client cl = findClient(content[2]);
            subject++;
            size_t i = findChannel(content[1]);
            if (admin.GetOperator())
                _vecCh[i].addOnList(cl);
            else
            {
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch(const std::exception& e)
        {
            if (subject == 0)
            {
                std::string ms = ":" + _ServName + " 442 :Not on channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
            else
            {
                std::string ms = ":" + _ServName + " 403 :No such channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
    }
    else
    {
        std::string ms = ":" + _ServName + " 461 :Need more params\r\n";
        send(admin.getOut(), ms.c_str(), ms.size(), 0);
    }
}

void server::kickCmd(std::vector<std::string> content, client admin)
{
    if (content.size() > 2)
    {
        int subject = 0;
        try
        {
            client cl = findClient(content[2]);
            ++subject;

            size_t i = findChannel(content[1]);
            if (admin.GetOperator())
                _vecCh[i].kick(cl);
            else
            {
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            if (subject == 0)
            {
                std::string ms = ":" + _ServName + " 442 :Not on channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
            else
            {
                std::string ms = ":" + _ServName + " 403 :No such channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
    }
    else
    {
        std::string ms = ":" + _ServName + " 461 :Need more params\r\n";
        send(admin.getOut(), ms.c_str(), ms.size(), 0);
    }
}

void server::topicCmd(std::vector<std::string> cmd, client &cl)
{
    size_t pos = findChannel(cmd[1]);
    if (cmd.size() == 3)
    {
        if (_vecCh[pos].isOnTheChannel(cl))
        {
            if (_vecCh[pos].getResTopic())
            {
                if (_vecCh[pos].isAdmin(cl))
                {
                    std::string str;
                    for (size_t i = 2; i < cmd.size(); i++)
                    {
                        str += cmd[i];
                        if (i + 1 < cmd.size())
                            str += " ";
                        else
                            str += "\n";
                    }
                    _vecCh[pos].setTopic(str);
                }
                else
                {
                    std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                    send(cl.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            else
            {
                std::string str;
                for (size_t i = 2; i < cmd.size(); i++)
                {
                    str += cmd[i];
                }
                _vecCh[pos].setTopic(str);
            }
        }
        else
        {
            std::string ms = ":" + _ServName + " 442 :Not on channel\n";
            send(cl.getOut(), ms.c_str(), ms.size(), 0);
        }
    }
    else
    {
        if (!_vecCh[pos].getTopic().empty())
        {
            send(cl.getOut(), _vecCh[pos].getTopic().c_str(), _vecCh[pos].getTopic().size(), 0);
        }
        else
        {
            std::string ms = ":" + _ServName + " 461 :Need more params\n";
            send(cl.getOut(), ms.c_str(), ms.size(), 0);
        }
    }
}

void server::modeCmd(std::vector<std::string> cmd, client admin)
{
    try
    {
        size_t i = findChannel(cmd[1]);
        if (_vecCh[i].isAdmin(admin))
        {
            if (cmd[2] == "-i")
                _vecCh[i].allowInvite();
            else if (cmd[2] == "-t")
                _vecCh[i].allowResTopic();
            else if (cmd[2] == "-k")
            {
                if (cmd.size() > 3)
                    _vecCh[i].allowkey(cmd[3]);
                else
                    _vecCh[i].UnsetKey();
            }
            else if (cmd[2] == "-o")
            {
                if (cmd.size() > 3)
                {
                    if (_vecCh[i].validUser(cmd[3]))
                        _vecCh[i].allowOperator(cmd[3]);
                    else
                    {
                        std::string ms = ":" + _ServName + " 401 :No such nickname\n";
                        send(admin.getOut(), ms.c_str(), ms.size(), 0);
                    }
                }
                else
                {
                    if (_vecCh[i].getResTopic() && !_vecCh[i].isAdmin(admin))
                    {
                        std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                        send(admin.getOut(), ms.c_str(), ms.size(), 0);
                    }
                    else
                        _vecCh[i].allowOperator(admin.GetClientUserName());
                }
            }
            else if (cmd[2] == "-l")
            {
                if (cmd.size() > 3)
                {
                    size_t nb = std::atoi(cmd[3].c_str());
                    if (nb >= _vecCh[i].getchannelClients().size())
                        _vecCh[i].setLimitCl(nb);
                    else
                    {
                        std::string ms = ":" + _ServName + " 501 :Limit too small\n";
                        send(admin.getOut(), ms.c_str(), ms.size(), 0);
                    }
                }
                else if (cmd.size() > 2)
                    _vecCh[i].UnsetLimitCl();
                else
                {
                    std::string ms = ":" + _ServName + " 501 :Mode unknow flag\n";
                    send(admin.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            else
            {
                std::string ms = ":" + _ServName + " 501 :Mode unknow flag\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        else
        {
            std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
            send(admin.getOut(), ms.c_str(), ms.size(), 0);
        }
    }
    catch(const std::exception& e)
    {
        std::string ms = ":" + _ServName + " 403 :No such channel\n";
        send(admin.getOut(), ms.c_str(), ms.size(), 0);
    }
}