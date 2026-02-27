#include "../libs/main.hpp"

int check_port(char *str)
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
std::string check_password(char *str)
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
        int port = check_port(av[1]);
        std::string password = check_password(av[2]);
        try {
            server serv(port, password);
            //TEST
            
            std::vector<struct pollfd> vec;
            struct pollfd tmp;
            tmp.fd = serv.getIdSocket();
            tmp.events = POLLIN;
            tmp.revents = 0;
            vec.push_back(tmp);
            /*struct pollfd tmp[1];
            tmp->fd = serv.getIdSocket();
            tmp->events = POLLIN;
            tmp->revents = 0;*/
            //FIN DU TEST
            while (1)
           {
                poll(&vec[0], vec.size(), 5000);
                if (vec[0].revents & POLLIN)
                {
                    std::cout << "check1" << std::endl;
                    client cl(port);
                    int fd_client = accept(serv.getIdSocket(), (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
                    if (fd_client != -1)
                    {
                        std::cout << "check2" << std::endl;
                        struct pollfd tmps;
                        tmps.fd = fd_client;
                        tmps.events = POLLIN;
                        tmps.revents = 0;
                        vec.push_back(tmps);
                    }
                    std::cout << "check3" << std::endl;
                }
                if (vec[0].revents & POLLERR)
                {
                    std::cout << "erreur err" << std::endl;
                }
                if (vec[0].revents & POLLHUP)
                {
                    std::cout << "erreur hup" << std::endl;
                }
                for (unsigned int i = 1; i < vec.size(); i++)
                {
                    char buf[10];
                    int nb = read(vec[i].fd, buf, 9);
                    buf[nb] = '\0';
                    std::cout << buf << std::endl;
                }
                /* 3 actions possibles:
                - un utilisateur se connect
                - un utilisateur se deco
                - un utilisateur envoi un message
                */
            }
            /*client cl(_Port);
            fd_client = accept(_IdSocket, (sockaddr *)&cl.SetClientInfo(), cl.GetClientSize());
            char buffer[15];
            int nb = read(fd_client, buffer, 14);
            buffer[nb] = '\0';
            std::cout << buffer;*/
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << 'n';
        }
    }
    else
    {
        std::cerr << "You need 2 arguments: 'port' & 'password'\n";
        return(1);
    }
    return(0);
}