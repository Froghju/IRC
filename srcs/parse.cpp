#include "../libs/class/server.hpp"
#include ".../libs/class/channel.hpp"
#include "../libs/main.hpp"
#include <stdio.h>

void server::joinCmd(std::vector content, client cl)
{
    for (std::vector<channel>::iterator it = _vecCh->begin(); it != _vecCh->end(); it++)
	{
        if ((*_vecCh)[i].sameName(content[1])) //a voir pour l'invite
        {
            if ((*_vecCh)[i].isPrivate)
                //message: Access denied. channel is private.
            else
                (*_vecCh)[i].addnewClient(cl);
        }   
		else
        {
            channel newchannel(content[1]);
            newchannel.addNewClient(cl); //fonction pas finie
            _vecCh.push_back(newchannel);
        }
	}
    /* check si le nom du channel existe
        si oui: rajouter le client au channel s'il a le droit
        si non: creer le serveur et le push dans le vecteur de channel
    */
}

void server::inviteCmd(std::string name, client cl)
{
    if (!cl.GetOperator())
        //message au client: acces denied
    else
    {
        for (std::vector<client>::iterator it = _vecCl->begin(); it != _vecCl->end(); it++)
        {
            if ((*_vecCl)[i].GetClientUserName() == name)
                //invite
        }
    }
    /* Verifier si le client est bien admin
    si non: acces denied
    si oui: envoyer un message d'invitation au client selectione ?*/
}

void server::kickCmd(std:string name, client cl)
{
    if (!cl.GetOperator())
        //message au client: acces denied
    else
    {
        for (std::vector<client>::iterator it = _vecCl->begin(); it != _vecCl->end(); it++)
        {
            if ((*_vecCl)[i].GetClientUserName() == name)
            {
                _vecCl.erase(it);
                break;
            }
        }
    }
    /*verifier si le client est bien admin
    si non: acces denied
    si oui: kick*/
}

void server::topicCmd()
{}

void server::modeCmd(std::vector<string> cmd, chanel ch)
{
    if (cmd[1] == 'i')
    {}
    else if (cmd[1] == 't')
    {}
    else if (cmd[1] == 'k')
    {}
    else if (cmd[1] == 'o')
    {}
    else if (cmd[1] == 'l')
    {}
    else
    {}
}

//fonction a transposer dans le server sinon pas acces aux variables private
void server::parse(std::string message)
{
    std::vector<std::string> sentence = splitCpp(message);

    if (!message.empty())
    {
        if (sentence[0] == "JOIN")
        {
            printf("cmd JOIN\n");
            if (sentence.size() > 2)
                joinCmd(sentence);
            else
                //message erreur: invalid command
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
                //message erreur: invalid command
        }
        else
        {
            printf("no parse\n");
        }
    }
}