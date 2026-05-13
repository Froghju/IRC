#include "../libs/main.hpp"

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
    return cmd;
}

std::string read_mess(client &cl)
{
    char buff[2];
    int nb;
    std::string text;
    while (1)
    {
        nb = recv(cl.getOut(), buff, 1, 0);
        if (nb == -1)
        {
            break;
        }
        buff[nb] = '\0';
        if (buff[0] == '\0')
        {
            break;
        }
        if (buff[0] == '\n' || buff[0] == '\r')
        {
            text.append(buff);
            break;
        }
        text.append(buff);
    }
    if (nb <= 0)
    {
        if ( nb == 0)
        {
            std::cerr << "Client Disconected from the server" << std::endl;
            text.clear();
            throw ClientQuit();
        }
        else
        {
            std::cerr << "Sorry fail of recv" << std::endl;
            throw ClientQuit();
        }
        shutdown(cl.getOut(), SHUT_RDWR);
        close(cl.getOut());
        return "";
    }
    buff[nb] = '\0';
    std::string all_text = cl.conCat(text.c_str());
    std::string::size_type pos = all_text.find('\r');
    if (pos == std::string::npos)
    {
        pos = all_text.find('\n');
    }
    std::string mess = all_text.substr(0, pos);
    all_text.erase(0, pos + 1);
    cl.resetMess(all_text);
    return mess;
    if (all_text.empty())
    {
        cl.resetMess("");
    }
    return "";
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

std::vector<std::string> splitCpp(std::string str)
{
    std::vector<std::string> cut;
    char *tmp;
    int i = 0;
    size_t pos = 0;
    int len = 0;
    bool check = false;

    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\r')
        {
            tmp = (char *)malloc(sizeof(char) * len + 1);
            if (check == false)
            {
                str.copy(tmp, len, pos);
                tmp[len] = '\0';
                cut.push_back(tmp);
            }
            else
            {
                str.copy(tmp, len, pos);
                tmp[len - 1] = '\0';
                cut.push_back(tmp);
            }
            pos = i + 1;
            len = 0;
            free(tmp);
            check = true;
        }
        i++;
        len++;
    }
    if (pos == 0)
        cut.push_back(str);
    else if (pos != str.size())
    {
        tmp = (char *)malloc(sizeof(char) * len);
        str.copy(tmp, len, pos);
        tmp[len - 1] = '\0';
        cut.push_back(tmp);
        free(tmp);
    }
    return cut;
}