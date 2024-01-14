#include "Server.hpp"

//EN TRAVAUX

// 	/PRIVMSG <target> {,<target>} <text to be sent>
// target peut etre le nom d'un user ou d'un channel

// replies possibles
// ERR_NOSUCHNICK (401)
// ERR_NOSUCHSERVER (402) ??
// ERR_CANNOTSENDTOCHAN (404)
// ERR_TOOMANYTARGETS (407)
// ERR_NORECIPIENT (411)
// ERR_NOTEXTTOSEND (412)
// ERR_NOTOPLEVEL (413)?
// ERR_WILDTOPLEVEL (414)?
// RPL_AWAY (301)?

void	Privmsg(Client *client, std::vector<std::string> args, Server &serv)
{
	if (check_command_access(client))
		return ;
	if (args.size() < 2)
	{
		client->send_reply(ERR_NORECIPIENT(client->get_hostname()));
		return ;
	}
	else if (!args[1].compare(0 , 1, "#"))
	{
		if (!serv.find_channel(args[1].erase(0, 0)))
		{
			client->send_message(ERR_NOSUCHNICK(client->get_nickname(), args[1]));
			return ;
		}
		else if (!serv.find_channel(args[1])->is_channelClient(client->get_nickname()))
		{
			client->send_message(ERR_NOTONCHANNEL(client->get_nickname(), args[1]));
			return ;
		}
		std::string message("");
		for (unsigned int i = 2; i < args.size();i++)
		{
			message += args[i];
			message += " ";
		}
		serv.find_channel(args[1].erase(0, 0))->send_privmsg_all(client->get_nickname(), ":" +client->get_fullname() + " PRIVMSG " + args[1]+ " " + message);
		return ;
	}
	else
	{
		if (!serv.find_client(args[1]))
		{
			client->send_message(ERR_NOSUCHNICK(client->get_nickname(), args[1]));
			return ;
		}
		else if (args[1] == client->get_nickname())
			return ;
		std::string message(args[2]);
		if (args.size() >= 3)
			message += " ";
		for (unsigned int i = 3; i < args.size();i++)
		{
			message += args[i];
			if (i != args.size() - 1)
				message += " ";
		}
		serv.find_client(args[1])->send_privmessage_from(client, message);
	}
}