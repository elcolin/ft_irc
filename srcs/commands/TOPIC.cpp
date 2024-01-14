#include "Commands.hpp"

void	Topic(Client *client, std::vector<std::string> args, Server &server)
{
	Channel *chan;

	if (check_command_access(client))
		return ;
	if (args.size() < 2)
	{
		client->send_message(ERR_NEEDMOREPARAMS(client->get_hostname(), "TOPIC"));
		return ;
	}
	chan = server.find_channel(args[1]);
	if (!chan)
	{
		return ;
	}
	if (args.size() == 2)
	{
		if (server.find_channel(args[1])->get_topic() == "")
			client->send_message( ":localhost 331 "+ client->get_nickname() + " " + args[1] + " :" + "No topic is set" );
		else
			client->send_message( ":localhost 332 "+ client->get_nickname() + " " + args[1] + " " + chan->get_topic() );
	}
	else
	{
		if (chan->get_restriction_TOPIC_cmd() && !chan->is_operator(client->get_nickname()))
			client->send_message( client->get_nickname() + " " + chan->get_name() + " :You're not channel operator" );
		else
		{

			std::string topic(args[2]);
			if (args.size() > 3)
			{
				for ( std::vector<std::string>::iterator it = args.begin() + 3; it != args.end(); it++ )
				{
					topic += " ";
					topic += *it;
				}	
			}
			chan->set_topic(topic);
			chan->send_all_clients( ":localhost 332 "+ client->get_nickname() + " " + args[1] + " " + topic );
		}
	}

}