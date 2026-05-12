#include "../libs/main.hpp"

void server::sendjoin(size_t pos, client &cl)
{
	std::string str = ":" + cl.GetNickname() + "!" + cl.GetClientUserName() + "@localhost JOIN #" + _vecCh[pos].getname() + "\r\n";
	send(cl.getOut(), str.c_str(), str.size(), 0);
}

void server::sendlistclchannel(size_t pos)
{
	std::string str;
	for (std::vector<client>::iterator it = _vecCh[pos].getchannelClients().begin(); it != _vecCh[pos].getchannelClients().end(); ++it)
	{
		if (_vecCh[pos].isAdmin(*it))
			str +="@";
		str += it->GetNickname();
		if (it + 1 != _vecCh[pos].getchannelClients().end())
			str += " ";
		else
			str += " Frogy\r\n";
	}
	std::cerr << "list = " << str << std::endl;
	for (std::vector<client>::iterator it = _vecCh[pos].getchannelClients().begin(); it != _vecCh[pos].getchannelClients().end(); ++it)
	{
		std::string mess = ":" + _ServName + " 353 " + it->GetNickname() + " = #" + _vecCh[pos].getname() + " :" + str;
		send(it->getOut(), mess.c_str(), mess.size(), 0);
		std::string mess2 = ":" + _ServName + " 366 " + it->GetNickname() + " #" + _vecCh[pos].getname() + " :End of /NAMES list\r\n";
		send(it->getOut(), mess.c_str(), mess.size(), 0);
	}
}

void channel::sendoperator(client &cl)
{
	std::string mess = ":Tha_Ghj MODE #" + _name + " +o " + cl.GetNickname() + "\r\n";
	for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); ++it)
	{
		send(it->getOut(), mess.c_str(), mess.size(), 0);
	}
	/*send(cl.getOut(), mess.c_str(), mess.size(), 0);
	std::string mess2 = ":" + _ServName + " 353 " + cl.GetNickname() + " = #" + _vecCh[pos].getname() + " :@" + cl.GetNickname() + "\r\n";
	send(cl.getOut(), mess.c_str(), mess.size(), 0);*/
}

void channel::unsendoperator(client &cl)
{
	std::string mess = ":Tha_Ghj MODE #" + _name + " -o " + cl.GetNickname() + "\r\n";
	for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); ++it)
	{
		send(it->getOut(), mess.c_str(), mess.size(), 0);
	}
	/*send(cl.getOut(), mess.c_str(), mess.size(), 0);
	std::string mess2 = ":" + _ServName + " 353 " + cl.GetNickname() + " = #" + _vecCh[pos].getname() + " :@" + cl.GetNickname() + "\r\n";
	send(cl.getOut(), mess.c_str(), mess.size(), 0);*/
}

void server::sendTopic(size_t pos, client &cl)
{
	std::string mess = ":" + _ServName + " 332 " + cl.GetNickname() + " #" + _vecCh[pos].getname() + " " + _vecCh[pos].getTopic() + "\r\n";
	std::cerr << "mess topic = " << mess << std::endl;
	for (size_t i = 0; i < mess.size(); i++)
	{
		std::cout << (int)(unsigned char)mess[i] << " ";
	}
	std::cout << std::endl;
	send(cl.getOut(), mess.c_str(), mess.size(), 0);
}

void server::sendTopicAll(size_t pos)
{
	for (std::vector<client>::iterator it = _vecCh[pos].getchannelClients().begin(); it != _vecCh[pos].getchannelClients().end(); ++it)
	{
		sendTopic(pos, *it);
	}
}

void server::sendNoTopic(size_t pos, client &cl)
{
	std::string mess = ":" + _ServName + " 331 " + cl.GetNickname() + " #" + _vecCh[pos].getname() + " :No topic is set\r\n";
	std::cerr << "mess no topic = " << mess << std::endl;
	for (size_t i = 0; i < mess.size(); i++)
	{
		std::cout << (int)(unsigned char)mess[i] << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < cl.GetNickname().size(); i++)
	{
		std::cout << (int)(unsigned char)cl.GetNickname()[i] << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < _vecCh[pos].getname().size(); i++)
	{
		std::cout << (int)(unsigned char)_vecCh[pos].getname()[i] << " ";
	}
	std::cout << std::endl;
	send(cl.getOut(), mess.c_str(), mess.size(), 0);
}

void server::sendNoTopicAll(size_t pos)
{
	for (std::vector<client>::iterator it = _vecCh[pos].getchannelClients().begin(); it != _vecCh[pos].getchannelClients().end(); ++it)
	{
		sendNoTopic(pos, *it);
	}
}

void server::sendInvite(client &cl, size_t pos)
{
	std::string mess = ":" + _ServName + " 341 " + cl.GetNickname() + " #" + _vecCh[pos].getname() + "\r\n";
	std::cerr << "mess = " << mess << std::endl;
	send(cl.getOut(), mess.c_str(), mess.size(), 0);
}

void server::sendInviteOnly(client &cl, size_t pos)
{
	std::string mess = ":" + _ServName + " 346 " + cl.GetNickname() + " #" + _vecCh[pos].getname() + "\r\n";
	send(cl.getOut(), mess.c_str(), mess.size(), 0);
}

void server::sendNewNick(client &cl, std::string newname)
{
	std::string mess = ":" + cl.GetNickname() + "!" + cl.GetClientUserName() + "@localhost NICK :" + newname + "\r\n";
	send(cl.getOut(), mess.c_str(), mess.size(), 0);
}
/*std::string ms3 = ":" + cl.GetNickname() + "!" + cl.GetClientUserName() + "@localhost JOIN #" + _vecCh[_vecCh.size() - 1].getname() + "\r\n";
send(cl.getOut(), ms3.c_str(), ms3.size(), 0);*/
/*std::string ms4 = ":" + _ServName + " MODE #" + _vecCh[_vecCh.size() - 1].getname() + " +o " + cl.GetNickname() + "\r\n";
send(cl.getOut(), ms4.c_str(), ms4.size(), 0);
std::string ms5 = ":" + _ServName + " 353 " + cl.GetNickname() + " = #" + _vecCh[_vecCh.size() - 1].getname() + " :@" + cl.GetNickname() + "\r\n";
send(cl.getOut(), ms5.c_str(), ms5.size(), 0);*/
/*std::string ms = ":" + _ServName + " 353 " + cl.GetNickname() + " = #" + _vecCh[_vecCh.size() - 1].getname() + " :" + cl.GetNickname() + "\r\n";
send(cl.getOut(), ms.c_str(), ms.size(), 0);
std::string ms2 = ":" + _ServName + " 366 " + cl.GetNickname() + " #" + _vecCh[_vecCh.size() - 1].getname() + " :End of /NAMES list\r\n";
send(cl.getOut(), ms2.c_str(), ms2.size(), 0);*/