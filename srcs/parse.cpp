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
    throw ;
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

void server::joinCmd(std::vector<std::string> content, client cl)
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
            (void)e;
            channel newchannel(content[1], content[2]);
            newchannel.addNewClient(cl);
            _vecCh.push_back(newchannel);
        }
    }
    else
        send(cl.GetFdOut(), "Invalid command: JOIN <channel> (<password>)\n", 44, 0);
}

//ICI Admin est le client a l'initiative de l'action
void server::inviteCmd(std::vector<std::string> content, client admin)
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
            (void)e;
            send(admin.GetFdOut(), "Invalid command: channel or client doesn't exist\n", 50, 0);
        }
    }
    else
        send(admin.GetFdOut(), "Invalid command: INVITE <channel> <user>\n", 42, 0);
}

//ICI Admin est le client a l'initiative de l'action
void server::kickCmd(std::vector<std::string> content, client admin)
{
    if (content.size() > 2)
    {
        try
        {
            client cl = findClient(content[2]);
            size_t i = findChannel(content[1]);
            if (admin.GetOperator())
                _vecCh[i].kick(cl);
            else
                send(cl.GetFdOut(), "You have no right to kick another user\n", 40, 0);
        }
        catch(const std::exception& e)
        {
            (void)e;
            send(admin.GetFdOut(), "Invalid command: This channel doesn't exist\n", 45, 0);
        }
    }
    else
        send(admin.GetFdOut(), "Invalid command: KICK <channel> <user>\n", 41, 0);
}

void server::modeCmd(std::vector<std::string> cmd, client admin)
{
    try
    {
        size_t i = findChannel(cmd[1]);
        if (cmd[2] == "i")
            _vecCh[i].allowInvite();
        /*else if (cmd[1] == "t")
        {
            if (cmd.size > 3)
                _vecCh[i].allowTopic(cmd[3]); //a faire THAIS
            else
                send(cl.GetFdOut(), "Invalid command: 'MODE <channel> -flag <name_of_the_topic>'\n", 61, 0);
        }*/
        else if (cmd[1] == "k")
            _vecCh[i].allowkey(cmd);
        /*else if (cmd[1] == "o") //THAIS
        {
            if (validUser(cmd[3]))
            {
                if (cmd.size() > 3)
                    _vecCh[i].allowOperator(cmd[3]); //a faire
                else
                    send(cl.GetFdOut(), "Invalid command: 'MODE <channel> -flag <name_of_the_user>'\n", 60, 0);
            }
            else
                send(cl.GetFdOut(), "This user doest't exist\n", 25, 0);
        }
        else if (cmd[1] == "l") // THAIS
        {
            if (cmd.size() > 3)
                    _vecCh[i].allowUserLimit(cmd[3]); //a faire
                else
                    send(cl.GetFdOut(), "Invalid command: 'MODE <channel> -flag <numbers_of_users>'\n", 60, 0);
        }*/
        else
            std::cerr << "error bad extention mode" << std::endl;
    }
    catch(const std::exception& e)
    {
        (void)e;
        send(admin.GetFdOut(), "Invalid command: This channel doesn't exist\n", 45, 0);
    }
}