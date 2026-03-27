#include "../libs/main.hpp"

std::string read_mess(int fd)
{
    std::string all_text;
    int nb = 0;
    char buffer[2];
    while (1)
    {
        nb = recv(fd, buffer, 1, 0);
        if (nb == -1)
        {
            send(fd, "Sorry fail of recv you leave the serv\n", 39, 0);
            return NULL;
        }
        buffer[nb] = '\0';
        if (buffer[0] == '\0' || (buffer[0] == '\n' && (buffer[1] == '\0' || buffer[1] == '\r')))
            break;
        buffer[nb] = '\0';
        all_text += buffer;
    }
    return (all_text);
}