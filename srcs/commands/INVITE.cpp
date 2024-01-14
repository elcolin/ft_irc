//****************************************************//
//                  Commande INVITE                   //
//****************************************************//

#include "Commands.hpp"

// "/INVITE [user.nickname]" Invite un client sur le channel de l'utilisateur.

void	Invite( Client *client, std::vector<std::string> received, Server &server )
{
	if ( invite_error( client, received, server) )
		return;

	Channel* channel = server.find_channel( received[2] );
	Client *target = server.find_client( received[1] );
	std::string target_nick = target->get_nickname();

	// Reply from user command
	client->send_reply( RPL_INVITING( client->get_nickname(), target_nick, channel->get_name() ) );
	client->send_message( client->get_nickname() + " INVITE " + target_nick + " " + channel->get_name() );

	// add client to channel
	channel->add_invite( received[1] );
	target->send_message( "You've been invited on " + channel->get_name() );
}

bool	invite_error( Client *client, std::vector<std::string> received, Server &server )
{
	std::string nickname = client->get_nickname();

	if (check_command_access(client))
		return ( true );

	if ( received.size() != 3 )
		return ( client->send_reply( ERR_NEEDMOREPARAMS( nickname, "INVITE" ) ), true );

	Channel* channel = server.find_channel( received[2] );

	if ( received[1] == nickname )
		return( client->send_reply( "Error: you can't invite yourself" ), true );

	else if ( !server.find_client( received[1] ) )
		return ( client->send_reply( ERR_NOSUCHNICK( nickname, received[1] ) ), true );

	else if ( !channel )
		return ( client->send_reply( ERR_NOSUCHCHANNEL( nickname, received[2] ) ), true );

	else if ( !channel->is_channelClient( nickname ) )
		return ( client->send_reply( ERR_NOTONCHANNEL( nickname, channel->get_name() ) ), true );

	else if ( !channel->is_operator( nickname ) )
		return ( client->send_reply( nickname + " " + channel->get_name() + ": You're not channel operator"), true );

	else if ( channel->is_channelClient( server.find_client( received[1] )->get_nickname() ) )
		return ( client->send_reply( ERR_USERONCHANNEL( nickname, server.find_client( received[1] )->get_nickname(),  channel->get_name() ) ), true );

	else if ( channel->get_user_limit() && (int)channel->get_channelClients().size() >= channel->get_user_limit_nb() )
		return ( client->send_reply( client->get_nickname() + " " + channel->get_name() + ": Channel is full" ), true );

	return ( false );
}
