#include "Commands.hpp"

int check_command_access(Client *client)
{
	if (!check_auth(client))
	{
		client->send_message(ERR_AUTH);
		client->set_register(NOT_REGISTERED);
		return 1;
	}
	return 0;
}

void	Cap(Client *client, std::vector<std::string> args, Server &serv)
{
	(void)args;
	(void)serv;
	if (client->get_registered() == NOT_REGISTERED)
		client->set_register(AWAITING_AUTH);
	return ;
}