#include "../libs/class/server.hpp"
#include "../libs/class/channel.hpp"
#include "../libs/main.hpp"

size_t server::findChannel(std::string name)
{
    size_t i = 0;
    std::string tmp;
    std::cerr << "channel = " << name << std::endl;
    if (!name.empty() && name[0] == '#')
    {
        for (size_t i = 1; i < name.size(); ++i)
            tmp += name[i];
        std::cerr << "tmp = " << tmp << std::endl;
        while (i < _vecCh.size())
        {
            if (_vecCh[i].sameName(tmp))
                return i;
            i++;
        }
    }
    else
    {
        while (i < _vecCh.size())
        {
            if (_vecCh[i].sameName(name))
                return i;
            i++;
        }
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
                    return;
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
                        return;
                    }
                }
                else
                {
                    std::string ms = ":" + _ServName + " 461 :Need more params\r\n";
                    send(cl.getOut(), ms.c_str(), ms.size(), 0);
                    return;
                }
            }
            if (_vecCh[i].isPrivate())
            {
                if (_vecCh[i].isOnTheList(cl))
                {
                    _vecCh[i].addNewClient(cl);
                    sendjoin(i, cl);
                    if (!_vecCh[i].getTopic().empty())
                        sendTopicAll(i);
                    else
                        sendNoTopicAll(i);
                    sendlistclchannel(i);
                }
                else
                {
                    std::string ms = ":" + _ServName + " 473 :Invite only chan\r\n";
                    send(cl.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            else
            {
                _vecCh[i].addNewClient(cl);
                sendjoin(i, cl);
                if (!_vecCh[i].getTopic().empty())
                    sendTopicAll(i);
                else
                    sendNoTopicAll(i);
                sendlistclchannel(i);
                return;
            }
        }
        catch (const std::exception &e)
        {
            channel newchannel(content);
            newchannel.addNewClient(cl);
            _vecCh.push_back(newchannel);
            size_t pos = _vecCh.size() - 1;
            sendjoin(pos, cl);
            sendNoTopic(pos, cl);
            sendlistclchannel(pos);
            _vecCh[pos].sendoperator(cl);
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
        std::cerr << "check 0" << std::endl;
        int subject = 0;
        try
        {
            std::cerr << "check 1" << std::endl;
            client cl = findClient(content[1]);
            std::cerr << "check 2" << std::endl;
            subject++;
            size_t i = findChannel(content[2]);
            std::cerr << "check 3" << std::endl;
            if (admin.GetOperator())
            {
                std::cerr << "check 4" << std::endl;
                _vecCh[i].sendToAll(admin, content);
                sendInvite(cl, i);
                _vecCh[i].addOnList(cl);
            }
            else
            {
                std::cerr << "check 5" << std::endl;
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "check 6" << std::endl;
            if (subject == 0)
            {
                std::cerr << "check 7" << std::endl;
                std::string ms = ":" + _ServName + " 442 :Not on channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
            else
            {
                std::cerr << "check 8" << std::endl;
                std::string ms = ":" + _ServName + " 403 :No such channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
    }
    else
    {
        std::cerr << "check 9" << std::endl;
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
            std::cerr<< "check 0" << std::endl;
            client cl = findClient(content[2]);
            ++subject;
            std::cerr<< "check 1" << std::endl;
            size_t i = findChannel(content[1]);
            if (admin.GetOperator())
            {
                std::cerr<< "check 2" << std::endl;
                _vecCh[i].sendToAll(admin, content);
                _vecCh[i].kick(cl);
                sendlistclchannel(i);
            }
            else
            {
                std::cerr<< "check 3" << std::endl;
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            if (subject == 0)
            {
                std::cerr<< "check 4" << std::endl;
                std::string ms = ":" + _ServName + " 442 :Not on channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
            else
            {
                std::cerr<< "check 5" << std::endl;
                std::string ms = ":" + _ServName + " 403 :No such channel\r\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
    }
    else
    {
        std::cerr<< "check 6" << std::endl;
        std::string ms = ":" + _ServName + " 461 :Need more params\r\n";
        send(admin.getOut(), ms.c_str(), ms.size(), 0);
    }
}

void server::topicCmd(std::vector<std::string> cmd, client &cl)
{
    size_t pos = findChannel(cmd[1]);
    std::cerr << "check topic" << std::endl;
    if (cmd.size() == 3)
    {
        std::cerr << "check topic 2" << std::endl;
        if (_vecCh[pos].isOnTheChannel(cl))
        {
            std::cerr << "check topic 3" << std::endl;
            if (_vecCh[pos].getResTopic())
            {
                std::cerr << "check topic 4" << std::endl;
                if (_vecCh[pos].isAdmin(cl))
                {
                    std::cerr << "check topic 5" << std::endl;
                    std::string str;
                    for (size_t i = 2; i < cmd.size(); i++)
                    {
                        str += cmd[i];
                        if (i + 1 < cmd.size())
                            str += " ";
                    }
                    _vecCh[pos].setTopic(str);
                    std::cerr << "check topic 6" << std::endl;
                    if (_vecCh[pos].getResTopic())
                        sendTopicAll(pos);
                    else
                        sendNoTopicAll(pos);
                }
                else
                {
                    std::cerr << "check topic 7" << std::endl;
                    std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                    send(cl.getOut(), ms.c_str(), ms.size(), 0);
                }
            }
            else
            {
                std::cerr << "check topic 8" << std::endl;
                std::string str;
                for (size_t i = 2; i < cmd.size(); i++)
                {
                    str += cmd[i];
                    if (i + 1 < cmd.size())
                        str += " ";
                }
                _vecCh[pos].setTopic(str);
                sendTopicAll(pos);
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
            std::cerr << "check topic 9" << std::endl;
            sendTopicAll(pos);
        }
        else
        {
            std::cerr << "check topic 10" << std::endl;
            sendNoTopicAll(pos);
        }
    }
}

void server::modeCmd(std::vector<std::string> cmd, client admin)
{
    try
    {
        if (cmd.size() > 2)
        {
            size_t i = findChannel(cmd[1]);
            if (_vecCh[i].isAdmin(admin))
            {
                if (cmd[2] == "+i")
                    _vecCh[i].setInviteOnly();
                else if (cmd[2] == "-i")
                    _vecCh[i].unsetInviteOnly();
                else if (cmd[2] == "-t")
                    _vecCh[i].unsetResTopic();
                else if (cmd[2] == "+t")
                    _vecCh[i].setResTopic();
                else if (cmd[2] == "-k")
                {
                    _vecCh[i].UnsetKey(cmd);
                }
                else if (cmd[2] == "+k")
                {
                    _vecCh[i].setKey(cmd);
                }
                else if (cmd[2] == "+o")
                {
                    _vecCh[i].allowOperator(cmd);
                    sendoperator(i, findClient(cmd[3]));
                    sendlistclchannel(i);
                }
                else if (cmd[2] == "-o")
                {
                    _vecCh[i].unallowOperator(cmd);
                    unsendoperator(i, findClient(cmd[3]));
                    sendlistclchannel(i);
                }
                else if (cmd[2] == "-l")
                {
                    _vecCh[i].UnsetLimitCl();
                }
                else if (cmd[2] == "+l")
                {
                    if (cmd.size() == 4)
                    {
                        char *end;
                        double val;
                        val = std::strtod(cmd[3].c_str(), &end);
                        int v;
                        v = static_cast<int>(val);
                        _vecCh[i].setLimitCl(v);
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
    }
    catch (const std::exception &e)
    {
        std::string ms = ":" + _ServName + " 403 :No such channel\n";
        send(admin.getOut(), ms.c_str(), ms.size(), 0);
    }
}

void server::eraseClient(client &cl)
{
    std::cerr << cl.GetNickname() << " has been erased" << std::endl;
    shutdown(cl.GetClientID(), SHUT_RDWR);
    close(cl.GetClientID());
    std::vector<client>::iterator it = std::find(_vecCl.begin(), _vecCl.end(), cl);
    std::cerr << "IT : " << it->GetNickname() << std::endl;
    _vecCl.erase(it);
}