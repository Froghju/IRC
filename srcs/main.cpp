#include "../libs/main.hpp"
#include <stdio.h>

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

void exec(int port, std::string password)
{
    server serv(port, password);
}

//void    init
int main(int ac,char **av)
{
    if (ac == 3)
    {
        int port = checkPort(av[1]);
        std::string password = checkPassword(av[2]);
        printf("A");
        try
        {
            printf("B");
            server serv(port, password);
            printf("C");
            std::vector<struct pollfd> vec;
            printf("D");
            vec.push_back(serv.GetPollFd());
            printf("E");

            while (1)
            {
                poll(&vec[0], vec.size(), 5000);
                serv.checkPollRevents(&vec);
            }
            printf("F");
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "LALALALA" << std::endl;
        }
    }
    else
    {
        std::cerr << "You need 2 arguments: 'port' & 'password'\n";
        return(1);
    }
    return(0);
}



/*for (unsigned int i = 1; i < vec.size(); i++)
                {
                    if (vec[i].events != 0)
                    {
                        if (vec[i].revents & POLLHUP)
                        {
                            std::cout << "erreur pollhup" << std::endl;
                        }
                        if (vec[i].revents & POLLERR)
                        {
                            std::cout << "erreur pollerr" << std::endl;
                        }
                        if (vec[i].revents & POLLIN)
                        {
                            std::string test;
                            int nb = 0;
                            int check = 0;
                            while (nb <= 0)
                            {
                                char buff[10];
                                check = 1;
                                nb = read(vec[i].fd, buff, 9);
                                if (nb == -1)
                                {
                                    std::cout << "erreur" << std::endl;
                                    break;
                                }
                                else
                                {
                                    buff[nb] = '\0';
                                    test += buff;
                                }
                            }
                            if (nb != -1)
                                std::cout << test;
                        }
                        vec[i].revents = 0;
                    }*/