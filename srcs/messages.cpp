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
}

void channel::unsendoperator(client &cl)
{
	std::string mess = ":Tha_Ghj MODE #" + _name + " -o " + cl.GetNickname() + "\r\n";
	for (std::vector<client>::iterator it = _channelClients.begin(); it != _channelClients.end(); ++it)
	{
		send(it->getOut(), mess.c_str(), mess.size(), 0);
	}
}

void server::sendTopic(size_t pos, client &cl)
{
	std::string mess = ":" + _ServName + " 332 " + cl.GetNickname() + " #" + _vecCh[pos].getname() + " " + _vecCh[pos].getTopic() + "\r\n";
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


void server::sendlistclchannel(channel &chan)
{
	std::string str;
	for (std::vector<client>::iterator it = chan.getchannelClients().begin(); it != chan.getchannelClients().end(); ++it)
	{
		if (chan.isAdmin(*it))
			str +="@";
		str += it->GetNickname();
		if (it + 1 != chan.getchannelClients().end())
			str += " ";
		else
			str += " Frogy\r\n";
	}
	for (std::vector<client>::iterator it = chan.getchannelClients().begin(); it != chan.getchannelClients().end(); ++it)
	{
		std::string mess = ":" + _ServName + " 353 " + it->GetNickname() + " = #" + chan.getname() + " :" + str;
		send(it->getOut(), mess.c_str(), mess.size(), 0);
		std::string mess2 = ":" + _ServName + " 366 " + it->GetNickname() + " #" + chan.getname() + " :End of /NAMES list\r\n";
		send(it->getOut(), mess.c_str(), mess.size(), 0);
	}
}

void server::sendlistclallchannel(client &cl)
{
	for (std::vector<channel>::iterator it = _vecCh.begin(); it != _vecCh.end(); ++it)
	{
		if (it->isOnTheChannel(cl))
		{
			sendlistclchannel(*it);
		}
	}
}

void server::sendlistclall()
{
	for (std::vector<channel>::iterator it = _vecCh.begin(); it != _vecCh.end(); ++it)
	{
			sendlistclchannel(*it);
	}
}