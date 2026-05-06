#include "../libs/main.hpp"
#include <stdio.h>

volatile sig_atomic_t stop = 0;

int checkPort(char *str)
{
    size_t i = 0;
    size_t len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (!isdigit(str[i]))
        {
            std::cerr << "Invalid port information\n";
            exit(1);
        }
    }

    int port = atoi(str);
    
    if (port < 6660 || port > 7000)
    {
        std::cerr << "By default port must be between 6660 and 7000\n";
        exit(1);
    }
    return port;
}

std::string checkPassword(char *str)
{
    size_t i = 0;
    size_t len = strlen(str);
    std::string pass;

    if (len > 12)
    {
        std::cerr << "You can't set a password over 12 characters\n";
        exit(1);
    }
    for (i = 0; i < len; i++)
    {
        if (str[i] < 33 || str[i] > 126)
            exit(1);
        pass.push_back(str[i]);
    }
    return pass;
}

void sigint_handler(int sig)
{
    if (sig == SIGINT || sig == SIGTERM)
        stop = 1;
}

void set_sig_action(void)
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = &sigint_handler;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);

    /*struct sigaction pipeAct;
    memset(&pipeAct, 0, sizeof(pipeAct));
    pipeAct.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &pipeAct, NULL);*/
}

#include <signal.h>
int main(int ac,char **av)
{
    if (ac == 3)
    {
        set_sig_action();
        srand(time(NULL));
        int port = checkPort(av[1]);
        std::string password = checkPassword(av[2]);

        try
        {
            server serv(port, password);
            std::vector<struct pollfd> vec;
            vec.push_back(serv.GetPollFd());

            while (stop == 0)
            {
                poll(&vec[0], vec.size(), 5000);
                serv.checkPollRevents(&vec);
            }
            throw ErrorQuit();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "You need 2 arguments: 'port' & 'password'\n";
        return(1);
    }
    return(0);
}