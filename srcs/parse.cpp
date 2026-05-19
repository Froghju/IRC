#include "../libs/class/server.hpp"
#include "../libs/class/channel.hpp"
#include "../libs/main.hpp"

size_t server::findChannel(std::string name)
{
    size_t i = 0;
    if (!name.empty() && name[0] == '#')
    {
        while (i < _vecCh.size())
        {
            if (_vecCh[i].sameName(name))
            {
                return i;
            }
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
                    _vecCh[i].sendHistMsg(cl.getOut());
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
                _vecCh[i].sendHistMsg(cl.getOut());
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
            if (content[1][0] == '#')
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
            else
            {
                std::string ms = ":" + _ServName + " 461 :Params not ok\n";
                send(cl.getOut(), ms.c_str(), ms.size(), 0);
            }
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
            client cl = findClient(content[1]);
            subject++;
            size_t i = findChannel(content[2]);
            if (_vecCh[i].isAdmin(admin))
            {
                _vecCh[i].sendToAll(admin, content);
                sendInvite(cl, i);
                _vecCh[i].addOnList(cl);
            }
            else
            {
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch (const std::exception &e)
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
            if (_vecCh[i].isAdmin(admin))
            {
                if (_vecCh[i].isAdmin(cl))
                    _vecCh[i].unsendoperator(cl);
                _vecCh[i].sendToAll(admin, content);
                _vecCh[i].sendToMe(admin, content);
                _vecCh[i].kick(cl);
                sendlistclchannel(i);
            }
            else
            {
                std::string ms = ":" + _ServName + " 482 :Channel operator privilege needed\n";
                send(admin.getOut(), ms.c_str(), ms.size(), 0);
            }
        }
        catch (const std::exception &e)
        {
            (void)e;
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
    try
    {
        size_t pos = findChannel(cmd[1]);
        if (pos >= _vecCh.size())
            return ;
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
                        }
                        _vecCh[pos].setTopic(str);
                        if (_vecCh[pos].getResTopic())
                            sendTopicAll(pos);
                        else
                            sendNoTopicAll(pos);
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
        else if (cmd.size() == 2)
        {
            if (!_vecCh[pos].getTopic().empty())
                sendTopicAll(pos);
            else
                sendNoTopicAll(pos);
        }
        else
            ;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
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
                    sendlistclchannel(i);
                }
                else if (cmd[2] == "-o")
                {
                    _vecCh[i].unallowOperator(cmd);
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

void server::eraseClientChannel(client &cl)
{
    if (_vecCh.size() > 0)
    {
        for (std::vector<channel>::iterator it = _vecCh.begin(); it != _vecCh.end(); ++it)
        {
            if (it->isOnTheChannel(cl))
            {
                if (it->isOnTheList(cl))
                {
                    it->getchannelList().erase(find(it->getchannelList().begin(), it->getchannelList().end(), cl));
                }
                if (it->isAdmin(cl))
                {
                    it->getchannelAdmin().erase(find(it->getchannelAdmin().begin(), it->getchannelAdmin().end(), cl));
                }
                it->getchannelClients().erase(find(it->getchannelClients().begin(), it->getchannelClients().end(), cl));
            }
        }
    }
}

void server::eraseClient(client &cl)
{
    if (cl.GetReady())
    {
        eraseClientChannel(cl);
        std::string hex_mess = ":" + cl.GetNickname() + "!" + cl.GetClientUserName() + "@localhost QUIT :Connection closed\r\n";
        for (std::vector<client>::iterator it = _vecCl.begin(); it != _vecCl.end();++it)
        {
            if (it->GetReady())
                send(it->getOut(), hex_mess.c_str(), hex_mess.size(), 0);
        }
        sendlistclall();
    }
    shutdown(cl.GetClientID(), SHUT_RDWR);
    close(cl.GetClientID());
    std::vector<client>::iterator it = std::find(_vecCl.begin(), _vecCl.end(), cl);
    _vecCl.erase(it);
}