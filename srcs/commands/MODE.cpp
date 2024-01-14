//****************************************************//
//                   Commande MODE                    //
//****************************************************//

#include "Commands.hpp"

void	Mode(Client *client, std::vector<std::string> received, Server &server)
{
	if ( received.size() < 3 )
		return ( client->send_reply( ERR_NEEDMOREPARAMS( client->get_nickname(), "MODE" ) ) );

	Channel *channel = server.find_channel( received[1] );

	if ( received[1] == client->get_nickname() )
		return ;

	if ( !channel )
		return ( client->send_reply( ERR_NOSUCHCHANNEL( client->get_nickname(), received[1] ) ) ); 

	if (check_command_access(client))
		return ;
	else if (received.size() < 3)
		return ;
	else if ( received[2][1] == 'i' && server.find_channel( received[1] ))
		mode_invite_only( server.find_channel( received[1] ), client, received[2][0]);
	else if ( received[2][1] == 'k' )
		mode_channel_key( server.find_channel( received[1] ), client, received);
	else if ( received[2][1] == 't' )
		mode_restricion_topic_cmd( server.find_channel( received[1] ), client, received);
	else if ( received[2][1] == 'l' )
		mode_limit_user( server.find_channel( received[1] ), client, received );
	else if ( received[2][1] == 'o' )
		mode_operator_privilege(server.find_channel( received[1] ), client, received);
}


// -i set/remove invite-only channel :	
//  "/MODE -i" désactive invite-only mode.
//  "/MODE +i" active invite-only mode.
// Envoie un message indiquant l'état du mode au client. Par defaut [désactivé]

void	mode_invite_only(Channel *current, Client *user, char sign)
{
	if ( sign == '+' )
	{
		current->set_invite_only(1);
		user->send_message_in_channel( current->get_name(), "Invite-only mode is ON" );
	}
	else if ( sign == '-' )
	{
		current->set_invite_only(0);
		user->send_message_in_channel( current->get_name(), "Invite-only mode is OFF" );
	}
}

// -k set/remove channel key (pw)
// "/MODE -k" desactive le channel-key;
// "/MODE +k [password]" active le channel-key, ou change le mdp si deja activé. 
// Envoie un message indiquant l'état du mode au client. Par defaut [désactivé]

void	mode_channel_key( Channel *current, Client *user, std::vector<std::string> received)
{
	if ( received[2][0] == '+' && (int)received.size() < 4 )
		return ( user->send_message_in_channel( current->get_name(), "Error: password missing" ) );

	if ( received[2][0] == '-' )
	{
		current->set_channel_key(0, "");
		user->send_message_in_channel( current->get_name(), "Channel key is OFF" );
	}
	else if ( current->get_channel_key() && received[2][0] == '+' )
	{
		current->set_channel_key(1, received[3]);
		user->send_message_in_channel( current->get_name(), "Channel key has been changed" );
	}
	else if ( !current->get_channel_key() && received[2][0] == '+' )
	{
		current->set_channel_key(1 , received[3]);
		user->send_message_in_channel( current->get_name(), "Channel key is ON" );
	}
}


// -t set/remove restriction of the topic command to channel operator
// "/MODE -t" désactive la restriction à tous les clients du Channel de changer le TOPIC.
// "/MODE +t" active la restriction à tous les clients du Channel de changer le TOPIC.
// Envoie un message indiquant l'état du mode au client. Par defaut [désactivé]

void	mode_restricion_topic_cmd(Channel *current, Client *user, std::vector<std::string> received)
{
	if ( received[2][0] == '+' )
	{
		current->set_restriction_TOPIC_cmd(1);
		user->send_message_in_channel( current->get_name(), "Restriction to TOPIC command is ON" );
	}
	else
	{
		current->set_restriction_TOPIC_cmd(0);
		user->send_message_in_channel( current->get_name(), "Restriction to TOPIC command is OFF" );
	}
}

// -l set/remove the user limit to Channel:
// "/MODE -l" désactive le nombre de Client admis dans le Channel.
// "/MODE -l [nb]" active le nombre de Client admis dans le Channel, ou change le nombre si deja activé.
// Envoie un message indiquant l'état du mode au client. Par defaut [désactivé]

void	mode_limit_user(Channel *current, Client *user, std::vector<std::string> received)
{

	if ( received[2][0] == '+' )
	{
		if ( received.size() < 4 )
			return ( user->send_reply( ERR_NEEDMOREPARAMS( user->get_nickname(), "MODE +l" ) ) );

		if ( !isdigit( received[3][0] ) || received[3][0] == '0')
			return ( user->send_message_in_channel( current->get_name(), "Error : argument must be a positive number" ) );

		int limit_nb = atoi( received[3].c_str() );
		
		if ( limit_nb < (int)current->get_channelClients().size() )
			user->send_message_in_channel( current->get_name(), "Error : the limit cannot be less than the current number of users in that channel" );
		else
		{
			std::stringstream ss;
			ss << limit_nb;
			std::string limit_number_str = ss.str();

			current->set_user_limit( limit_nb );
			user->send_message_in_channel( current->get_name(), "User limit is " + limit_number_str );
		}
	}
	else if ( received[2][0] == '-' )
	{
		if ( received.size() < 3 )
			return ( user->send_reply( ERR_NEEDMOREPARAMS( user->get_nickname(), "MODE +l" ) ) );

		current->set_user_limit( 0 );
		user->send_message_in_channel( current->get_name(), "User limit is OFF" );
	}
}

// -o give/take channel operator privilege
// "MODE -o [target]" Si le client est au moins operator, il peut donner les pivilèges à n'importe qui.
// Seul le primordial peut retirer les privilèges. Envoie un message annonçant le nouvel operator, ou le client aayant été retrogradé.

void	mode_operator_privilege(Channel *current, Client *user, std::vector<std::string> received)
{
	if ( received.size() < 4	)
	{
		user->send_reply( ERR_NEEDMOREPARAMS( user->get_nickname(), "MODE" ) );
		return ;
	}
	if ( received[2][0] == '-' )
		current->operator_privilege(user, received[3], 0);
	else if ( received[2][0] == '+' )
		current->operator_privilege(user, received[3], 1);
} 