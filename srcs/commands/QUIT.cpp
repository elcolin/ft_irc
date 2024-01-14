#include "Commands.hpp"

void	Quit( Client *client, std::vector<std::string> args, Server &serv )
{
	(void)	serv;
	(void)	args;
	std::string reason = "";
	if (check_command_access(client))
		return ;
	if (args.size() != 1)
	{
		for (unsigned int i = 1; i < args.size() ; i++)
		{
			reason += args[i];
			if (i != args.size() - 1)
				reason += " ";
		}
	}
	serv.broadcast_server(":" + client->get_fullname() + " QUIT :" + reason);
	// quit_channels( client, serv );
	client->set_register(DISCONNECTED);
}

// void	quit_channels( Client *client, Server &serv )
// {
// 	std::vector<std::string> channel_list = client->get_channelList();

// 	for ( int i = 0; i < (int)channel_list.size(); i++ )
// 	{
// 		serv.find_channel( channel_list[i] )->kick_client( client->get_nickname() );
// 		for (int o = 0; o < (int)serv.find_channel( channel_list[i] )->get_channelClients().size(); o++ )
// 			std::cout << serv.find_channel( channel_list[i] )->get_channelClients()[o] << std::endl;
// 		for (int o = 0; o < (int)serv.find_channel( channel_list[i] )->get_operators().size(); o++ )
// 			std::cout << serv.find_channel( channel_list[i] )->get_operators()[o] << std::endl;
// 	}
// }
