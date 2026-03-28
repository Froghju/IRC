#include "../libs/main.hpp"

std::string read_mess(int fd)
{
    std::string all_text;
    int nb = 0;
    char buffer[2];
    int check = 0;
    while (1)
    {
        nb = recv(fd, buffer, 1, 0);
        if (nb == -1)
        {
            send(fd, "Sorry fail of recv you leave the serv\n", 39, 0);
            return NULL;
        }
        buffer[nb] = '\0';
        if (buffer[0] == '\0' || (buffer[0] == '\n' && buffer[1] == '\0'))
        {
            if (check == 0)
                all_text.append(buffer);
            break;
        }
        all_text.append(buffer);
        ++check;
    }
    return (all_text);
}

char *strTochar(std::string str) {
    char* buff = (char*)str.c_str();
    return (buff);
}

void sendToAll(int fd, std::vector<struct pollfd> *vec, std::string message, std::string name)
{
    int i = 1;
    if (message == "\n")
        message.append("\033[0m");
    else
        message.append("\n\033[0m");
    message.insert(0, name);
    message.insert(name.size(), " : \033[35m");
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        if ((*vec)[i].fd != fd)
            send((*vec)[i].fd, strTochar(message), message.size(), 0);
		i++;
	}
}