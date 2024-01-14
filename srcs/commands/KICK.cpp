//****************************************************//
//                   Commande KICK                    //
//****************************************************//

#include "Commands.hpp"

// "/KICK [user.nickname]" Le client kick un autre client sur le channel sur lequel il est.
// Le client doit au moins etre operateur pour pouvoir kick, 
// il ne peut pas kick d'autre operateur (s'il essaie un message d'erreur lui sera retourné).
// Un message confirmant son action lui sera envoyé, ainsi qu'au client ayant été sujet au kick.

// void	kick_command(Channel &current, Client &me, std::string target)
// {
// 	if (!target.size())
// 	{
// 		me.send_message( ERR_NEEDMOREPARAMS( me.get_nickname(), "KICK" ) );
// 		return;
// 	}
// 	if ( !current.is_channelClient( target ) )
// 	{
// 		me.send_message( ERR_NOTONCHANNEL( target, current.get_name() ) );
// 	}
// 	if ( !current.is_operator( me.get_nickname() ) )
// 	{
// 		me.send_message( ERR_CHANOPRIVSNEEDED( me.get_nickname(), current.get_name() ) );
// 		return;
// 	}
// 	if ( current.is_operator( target ) )
// 	{
// 		me.send_message( ERR_CANTKICKOPE( target, current.get_name() ) );
// 		return;
// 	}

// // remove client to channel's list
// 	current.kick_client( target );

// 	std::string message = target;
	
// 	target.append(" was kicked");
// 	me.send_message(message);
// }

void	kick( Client *client, std::vector<std::string> args, Server &server )
{
	if (check_command_access(client))
		return ;

	if ( args.size() < 3 )
	{
		client->send_reply( ERR_NEEDMOREPARAMS( client->get_nickname(), "KICK" ) );
		return ;
	}

	Channel *chan = server.find_channel( args[1] );

	if ( !chan )
	{
		client->send_reply( ERR_NOSUCHCHANNEL( client->get_nickname(), args[1] ) );
		return ;
	}
	if ( client->get_nickname() == args[2] )
	{
		client->send_reply( client->get_nickname() + " " + chan->get_name() + ": You can't kick yourself" );
		return ;
	}
	if ( !chan->is_channelClient( client->get_nickname() ) )
	{
		client->send_reply( ERR_NOTONCHANNEL( client->get_nickname(), chan->get_name() ) );
		return ;
	}
	if ( !chan->is_operator( client->get_nickname() ) )
	{
		client->send_reply( client->get_nickname() + " " + chan->get_name() + ": You're not channel operator" );
		return ;
	}
	if ( !chan->is_channelClient(args[2]) )
	{
		client->send_reply( args[2] + " " + chan->get_name() + " :They aren't on that channel" );
		return ;
	}
	if ( chan->is_operator( args[2] ) )
	{
		client->send_reply( ERR_CANTKICKOPE( args[2], chan->get_name() ) );
		return ;
	}
	std::string reason = ":No reason";
	if ( args[3].size() > 1)
	{
		reason = args[3];
		for ( std::vector<std::string>::iterator it = args.begin() + 4; it != args.end(); it++ )
		{
			reason += " ";
			reason += *it;
		}
	}
	chan->send_all_clients( ":" + client->get_fullname() + " KICK " + chan->get_name() + " " + args[2] + " " + reason );
	chan->kick_client( args[2] );
}
 