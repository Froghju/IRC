#include "../libs/main.hpp"

/*void send_hexchat_format(client cl, std::string mes)
{

}*/

bool is_white_space(char c)
{
    if (c == ' ' || (c >= 9 && c <= 13))
        return true;
    return false;
}

std::string find_input(std::string str, std::string cmd)
{
    size_t pos = str.find(cmd) + cmd.size();
    while (pos < str.size() && is_white_space(str[pos]))
        pos++;
    std::string input;
    input.append(str, pos, str.size());
    return input;
}

std::string find_cmd(std::string str)
{
    std::string cmd;
    size_t i = 0;
    std::cerr << "check " << std::endl;
    if (str[i] == ':')
    {
        while (i < str.size() && !is_white_space(str[i]))
            i++;
        while (i < str.size() && is_white_space(str[i]))
            i++;
    }
    int start = i;
    while (i < str.size() && !is_white_space(str[i]))
        i++;
    cmd.append(str, start, i);
    std::cerr << cmd << std::endl;
    return cmd;
}

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
    std::cerr << "all_text = " << all_text << std::endl;
    return (all_text);
}

char *strTochar(std::string str) {
    char* buff = (char*)str.c_str();
    return (buff);
}

void sendToAll(client &cl, std::vector<struct pollfd> *vec, std::string message, server &serv)
{
    int i = 1;
    std::string hex_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\r\n";
    std::string nc_mess = ":" + cl.GetNickname() +
                        "!~" + cl.GetClientUserName() +
                        "@localhost PRIVMSG #channel :" +
                        message + "\n";
    std::cerr << hex_mess << std::endl;
    for (std::vector<struct pollfd>::iterator it = vec->begin(); it != vec->end(); it++)
	{
        if ((*vec)[i].fd != cl.getOut())
        {
            if (serv.getVecCl()[i-1].getHex())
                send((*vec)[i].fd, hex_mess.c_str(), hex_mess.size(), 0);
            else
                send((*vec)[i].fd, nc_mess.c_str(), nc_mess.size(), 0);
        }
		i++;
	}
}