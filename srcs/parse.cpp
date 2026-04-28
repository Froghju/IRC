#include "../libs/class/server.hpp"
#include "../libs/class/channel.hpp"
#include "../libs/main.hpp"

size_t server::findChannel(std::string name)
{
    size_t i = 0;
    //std::cerr << "size vec channel " << _vecCh.size() << std::endl;
    //std::cerr << "name " << name << std::endl;  
    //std::cerr << "size vec channel " << _vecCh.size() << std::endl;
    //std::cerr << "name " << name << std::endl;  
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
void server::joinCmd(std::vector<std::string> content, client &cl)
{
    if (content.size() > 1)
    {
        try
        {
            size_t i = findChannel(content[1]);
            if (_vecCh[i].hasKey())
            {
                if (content.size() > 2)
                {
                    if (content[2] != _vecCh[i].getKey())
                    {
                        send(cl.GetFdOut(), "Invalid password\n", 18, 0);
                        return ;
                    }
                }
                else
                    send(cl.GetFdOut(), "This channel require a password\n", 33, 0);
            }
            if (_vecCh[i].isPrivate())
            {
                if (_vecCh[i].isOnTheList(cl))
                    _vecCh[i].addNewClient(cl);
                else
                    send(cl.GetFdOut(), "Sorry this channel is private\n", 31, 0);
            }
            else
            {
                _vecCh[i].addNewClient(cl);
                return ;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << ". Creating a new one..." << std::endl;
            std::cerr << e.what() << ". Creating a new one..." << std::endl;
            channel newchannel(content);
            newchannel.addNewClient(cl);
            _vecCh.push_back(newchannel);
        }
    }
    else
        send(cl.GetFdOut(), "Invalid command: JOIN <channel> (<password>)\n", 44, 0);
}

//ICI Admin est le client a l'initiative de l'action
void server::inviteCmd(std::vector<std::string> content, client &admin)
{
    if (content.size() > 2)
    {
        try
        {
            client cl = findClient(content[2]);
            size_t i = findChannel(content[1]);
            if (admin.GetOperator())
                _vecCh[i].addOnList(cl);
            else
                send(cl.GetFdOut(), "You have no right to invite another user\n", 42, 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            send(admin.GetFdOut(), "Invalid command: channel or client doesn't exist\n", 50, 0);
        }
    }
    else
        send(admin.GetFdOut(), "Invalid command: INVITE <channel> <user>\n", 42, 0);
}

//ICI Admin est le client a l'initiative de l'action
void server::kickCmd(std::vector<std::string> content, client admin)
{
    //SEGFAULT quand kick a user inexistant
    if (content.size() > 2)
    {
        int subject = 0;
        try
        {
            client cl = findClient(content[2]);
            ++subject;

            size_t i = findChannel(content[1]);
            std::cerr << admin.GetNickname() << " admin status: " << admin.GetOperator() << std::endl;
            if (admin.GetOperator()) //SWITCH dedans quand merge
                _vecCh[i].kick(cl);
            else
                send(admin.GetFdOut(), "You have no right to kick another user\n", 40, 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            if (subject == 0)
                send(admin.GetFdOut(), "Invalid command: This client doesn't exist\n", 44, 0);
            else
                send(admin.GetFdOut(), "Invalid command: This channel doesn't exist\n", 45, 0);
        }
    }
    else
        send(admin.GetFdOut(), "Invalid command: KICK <channel> <user>\n", 40, 0);
}

void server::topicCmd(std::vector<std::string> cmd, client &cl)
{
    size_t pos = findChannel(cmd[1]);
    if (cmd.size() == 3)
    {
        if (_vecCh[pos].getResTopic())
        {
            if (!_vecCh[pos].getResTopic() ||_vecCh[pos].isAdmin(cl))
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
                std::string str = "Error you don't have the permission to change topic";
                send(cl.getOut(), str.c_str(), str.size(), 0);
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
        if (!_vecCh[pos].getTopic().empty())
        {
            send(cl.getOut(), _vecCh[pos].getTopic().c_str(), _vecCh[pos].getTopic().size(), 0);
        }
        else
        {
            std::string str = "This channel have no topic\n";
            send(cl.getOut(), str.c_str(), str.size(), 0);
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
                _vecCh[i].allowkey(cmd, admin.GetFdOut());
            else if (cmd[2] == "-o")
            {
                if (cmd.size() > 3)
                {
                    if (_vecCh[i].validUser(cmd[3]))
                        _vecCh[i].allowOperator(cmd[3]);
                    else
                    {
                        std::string str = "Not valid user\n";
                        send(admin.GetFdOut(), str.c_str(), str.size(), 0);
                    }
                }
                else
                {
                    if (_vecCh[i].getResTopic() && !_vecCh[i].isAdmin(admin))
                    {
                        std::string str = "Can't be a operator\n";
                        send(admin.GetFdOut(), str.c_str(), str.size(), 0);
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
                    if (nb < _vecCh[i].getchannelClients().size())
                        _vecCh[i].setLimitCl(nb);
                    else
                    {
                        std::string str = "limit to small too many client in the channel\n";
                        send(admin.GetFdOut(), str.c_str(), str.size(), 0);
                    }
                }
                else
                    send(admin.GetFdOut(), "Invalid command: 'MODE <channel> -flag <numbers_of_users>'\n", 60, 0);
            }
            else
                std::cerr << "error bad extention mode" << std::endl;
        }
        else
            send(admin.GetFdOut(), "Permission denied: You are not a channel operator\n", 51, 0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        send(admin.GetFdOut(), "Invalid command: This channel doesn't exist\n", 45, 0);
    }
}