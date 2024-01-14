#include "Commands.hpp"

void	Ping(Client *client, std::vector<std::string> args, Server &server)
{
	(void)server;
	if (check_command_access(client))
		return ;
	else if (args.size() < 2)
	{
		client->send_message(ERR_NEEDMOREPARAMS(client->get_hostname(), "PING"));
		return ;
	}
	client->send_message("PONG " + args[1]);
		
}