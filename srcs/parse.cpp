#include "../libs/class/server.hpp"
#include "../libs/class/channel.hpp"
#include "../libs/main.hpp"

void server::joinCmd(std::vector<std::string> content, client cl)
{
    size_t i = 0;
    while (!_vecCh[i].sameName(content[1]))
        i++;
    if (i < _vecCh.size() && _vecCh[i].sameName(content[1]))
    {
        if (_vecCh[i].isPrivate())
        {
            //verrif if INVITE
            send(cl.GetFdOut(), "Sorry this channel is private\n", 31, 0);
        }
        else
        {
            _vecCh[i].addNewClient(cl);
            return ;
        }
    }
    channel newchannel(content[1], content[2]);
    newchannel.addNewClient(cl); //fonction pas finie
    _vecCh.push_back(newchannel);
}

void server::inviteCmd(std::string name, client cl)
{
    /*Verifier si le client est bien admin
    si non: acces denied
    si oui: envoyer un message d'invitation au client selectione ?*/
}

/*void server::kickCmd(std::string name, client cl)
{
    if (cl.GetOperator())
    {
        int before = //size du vector de client dans le channel;
        vec.erase(find(v.begin(), v.end(), name));
        if (before == size du vector)
            send(cl.GetFdOut(), "This user doesn't exist\n", 25, 0);
    }
    else
        send(cl.GetFdOut(), "You have no right to kick another user\n", 40, 0);*/
    /*verifier si le client est bien admin
    si non: acces denied
    si oui: kick
}*/

/*void server::topicCmd()
{}*/

void server::modeCmd(std::vector<std::string> cmd)
{
    if (cmd[1] == "i")
    {}
    else if (cmd[1] == "t")
    {}
    else if (cmd[1] == "k")
    {}
    else if (cmd[1] == "o")
    {}
    else if (cmd[1] == "l")
    {}
    else
    {}
}
#include <stdio.h>
void server::passCmd(client cl)
{
    std::string cmd = read_mess(cl.GetFdOut());

    if (!cmd.empty())
    {
        std::vector<std::string> pass = splitCpp(cmd);
        if (pass.size() == 2 && pass[0] == "PASS")
        {
            if (pass[1] == _PassW)
            {
                send(cl.GetFdOut(), "ok\n", 4, 0);
                cmd.clear();
                pass.erase(pass.begin(), pass.end());
            }
            else
            {
                send(cl.GetFdOut(), "Wrong password, disconnected from the server\n", 46, 0);
                cl.~client();
            }
        }
        while (pass[1] != _PassW)
		passCmd(cl.GetFdOut());
    }
}

//fonction a transposer dans le server sinon pas acces aux variables private
void server::parse(std::string message, client cl)
{
    std::vector<std::string> sentence = splitCpp(message);

    if (!message.empty())
    {
        if (sentence[0] == "JOIN")
        {
            //printf("cmd JOIN\n");
            if (sentence.size() > 2)
                joinCmd(sentence, cl);
            else
                send(cl.GetFdOut(), "Invalid command: JOIN <server_name>\n", 37, 0);
        }
        /*else if (sentence[0] == "INVITE")
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
        }*/
    }
}