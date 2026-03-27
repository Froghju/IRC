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
        all_text.append(buffer);
    }
    return (all_text);
}

char *strTochar(std::string str) {
    char* buff = (char*)str.c_str();
    return (buff);
}

void sendToAll(int fd, std::vector<struct pollfd> *vec, std::string message)
{
    int i = 1;
    message.insert(0, "\033[35m");
    message.append("\n\033[0m");
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        if ((*vec)[i].fd != fd)
            send((*vec)[i].fd, strTochar(message), message.size(), 0);
		i++;
	}
}