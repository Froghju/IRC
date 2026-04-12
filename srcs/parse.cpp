#include "../libs/main.hpp"
#include <stdio.h>

void joinCmd()
{}

void inviteCmd()
{}

void kickCmd()
{}

void topicCmd()
{}

void modeCmd()
{}

void parse(std::string message)
{
    std::vector<std::string> sentence = splitCpp(message);

    if (!message.empty())
    {
        if (sentence[0] == "JOIN")
        {
            printf("cmd JOIN\n");
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
        }
        else
        {
            printf("no parse\n");
        }
    }
}