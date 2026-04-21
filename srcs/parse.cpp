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

void server::joinCmd(std::vector<std::string> content, client cl) //PAS FINI EN FONCTION DE MODE
{
    try
    {
        size_t i = findChannel(content[1]);
        /* if (_vecCh[i].hasPass())
                if (content[2] != _vecCh[i].getKey())
                {
                    send(cl.GetFdOut(), "Invalid password\n", 18, 0);
                    return ;
                }*/
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
        newchannel.addNewClient(cl); //fonction pas finie
        _vecCh.push_back(newchannel);
    }
}

//ICI Admin est le client a l'initiative de l'action
void server::inviteCmd(std::string channel, client admin, client cl)
{
    int i = findChannel(channel);
    if (i > -1)
    {
        if (admin.GetOperator())
            _vecCh[i].addOnList(cl); //a faire
    }
    else
        send(cl.GetFdOut(), "Invalid command: channel doesn't exist\n", 40, 0);
}

//ICI Admin est le client a l'initiative de l'action
void server::kickCmd(std::string name, client admin, client cl)
{
    int i = findChannel(name);
    if (i > -1)
    {
        if (admin.GetOperator())
            _vecCh[i].kick(cl); //a faire
        else
            send(cl.GetFdOut(), "You have no right to kick another user\n", 40, 0);
    }
    else
        send(admin.GetFdOut(), "This channel doesn't exist\n", 28, 0);
}

/*void server::topicCmd()
{}*/

void server::modeCmd(std::vector<std::string> cmd, client cl)
{
    // cmd.size doit minimum etre de 3 !
    // 0: MODE; 1: <channel>; 2: flag; 3: Option(pas obligatoire pour tous)
    int i = findChannel(cmd[1]);
    if (i > -1 && cl.GetOperator())
    {
        if (cmd[2] == "i")
            _vecCh[i].allowInvite();
        /*else if (cmd[1] == "t")
        {
            if (cmd.size > 3)
                _vecCh[i].allowTopic(cmd[3]); //a faire
            else
                send(cl.GetFdOut(), "Invalid command: 'MODE <channel> -flag <name_of_the_topic>'\n", 61, 0);
        }
        else if (cmd[1] == "k")
            _vecCh[i].allowkey(cmd);
        else if (cmd[1] == "o")
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
        else if (cmd[1] == "l")
        {
            if (cmd.size() > 3)
                    _vecCh[i].allowUserLimit(cmd[3]); //a faire
                else
                    send(cl.GetFdOut(), "Invalid command: 'MODE <channel> -flag <numbers_of_users>'\n", 60, 0);
        }*/
        else
            std::cerr << "error bad extention mode" << std::endl;
    }
    if (i == -1)
        send(cl.GetFdOut(), "This channel doest't exist\n", 28, 0);
    else
        send(cl.GetFdOut(), "You have no right to change the topic\n", 39, 0);
}

void server::passCmd(std::string cmd, client &cl) //Possiblement useless
{
    if (!cmd.empty())
    {
        std::vector<std::string> pass = splitCpp(cmd);
        if (pass.size() == 2 && pass[0] == "PASS")
        {
            if (pass[1] == _PassW)
                send(cl.GetFdOut(), "ok\n", 4, 0);
            else
            {
                send(cl.GetFdOut(), "Wrong password, disconnected from the server\n", 46, 0);
                return ;
            }
        }
    }
}


//DEPRECATED, USED AS A REF
/*void server::parse(std::string message, client cl) 
{
    std::vector<std::string> sentence = splitCpp(message);

    if (!message.empty())
    {
        if (cl.GetStep() < 3)
        {
            if (cl.GetStep() == 0)
            {
                if (sentence[0] == "PASS")
                    passCmd(cl);
            }
            else
            {
                if (sentence[0] == "USER")
                    userCmd();
                if (sentence[0] == "NICK")
                    nickCmd();
            }
        }
        if (sentence[0] == "JOIN")
        {
            //printf("cmd JOIN\n");
            if (sentence.size() > 2)
                joinCmd(sentence, cl);
            else
                send(cl.GetFdOut(), "Invalid command: JOIN <server_name>\n", 37, 0);
        }
        else if (sentence[0] == "INVITE")
        {
            printf("cmd INVITE\n");
        }
        else if (sentence[0] == "KICK")
        {
            printf("cmd KICK\n");
        }
        else if (sentence[0] == "TOPIC")
        {
            printf("cmd TOPIC\n");
        }
        else if (sentence[0] == "MODE")
        {
            printf("cmd MODE\n");
            if (sentence.size() > 1)
                modeCmd(sentence);
            else
                send(cl.GetFdOut(), "Invalid command: MODE <flag>\n", 30, 0);
        }
        else
        {
            printf("no parse\n");
        }
    }
}*/