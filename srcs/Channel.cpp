#include "Channel.hpp"

//****************************************************//
//              Constructor/Destructor                //
//****************************************************//

Channel::Channel(std::string name, std::string primordial, Server &server):
_name(name), _primordial(primordial)
{
	_operators.push_back(primordial);
	_channelClients.push_back(primordial);
	_topic = "";
	_server = &server;
	_invite_only = 0;
	_channel_key = 0;
	_restriction_TOPIC_cmd = 1;
	_user_limit = 0;
	_user_limit_nb = 0;
}

Channel::~Channel() {}

//****************************************************//
//                      Setter                        //
//****************************************************//

void	Channel::set_invite_only(int i)
{
	if (!i)
		_invite_only = 0;
	else
		_invite_only = 1;
}

void	Channel::set_channel_key(int i, std::string password)
{
	if (!i)
		_channel_key = 0;
	else
	{
		_channel_key = 1;
		_password = password;
	}
}

void	Channel::set_restriction_TOPIC_cmd( int  i )
{
	if (!i)
		_restriction_TOPIC_cmd = 0;
	else 
		_restriction_TOPIC_cmd = 1;
}

void	Channel::set_user_limit( int limit )
{
	if (limit)
	{
		_user_limit = 1;
		_user_limit_nb = limit;
	}
	else
		_user_limit = 0;
}

void	Channel::set_topic(std::string newtopic)
{
	_topic = newtopic;
}

void	Channel::set_primo( std::string new_primo )
{
	_primordial = new_primo;
}

void	Channel::add_operator( std::string client )
{
	_operators.push_back( client );
}

void	Channel::add_invite( std::string invite )
{
	_invite_list.push_back( invite );
}

//****************************************************//
//                      Getter                        //
//****************************************************//

std::string	Channel::get_name()
{
	return (_name);
}

bool	Channel::get_invite_only()
{
	return (_invite_only);
}

bool	Channel::get_channel_key()
{
	return (_channel_key);
}

bool Channel::get_restriction_TOPIC_cmd()
{
	return (_restriction_TOPIC_cmd);
}

bool	Channel::get_user_limit()
{
	return (_user_limit);
}

int	Channel::get_user_limit_nb()
{
	return (_user_limit_nb);
}

Server *Channel::get_server()
{
	return (_server);
}

std::vector<std::string> Channel::get_channelClients()
{
	return (_channelClients);
}

std::vector<std::string> Channel::get_operators()
{
	return (_operators);
}

std::string	Channel::get_password()
{
	return (_password);
}

std::string	Channel::get_topic()
{
	return (_topic);
}

std::vector<std::string> Channel::get_invite()
{
	return (_invite_list);
}

//****************************************************//
//                     Function                       //
//****************************************************//

void	Channel::operator_privilege( Client *me, std::string target, int sign )
{
	if ( me->get_nickname() == target )
	{
		me->send_reply( me->get_nickname() + " " + _name + " :You can't use your nickname" );
		return;
	}

	if ( !is_operator( me->get_nickname() ) )
	{
		me->send_reply( me->get_nickname() + " " + _name + " :You're not channel's operator" );
		return;
	}

	if ( is_primordial( target ) )
	{
		me->send_reply( ERR_NOPRIMORDIAL( me->get_nickname(), _name ) );
		return;
	}

	if ( !is_channelClient( target ) )
	{
		me->send_reply( target + " " + _name + " :They aren't on that channel" );
		return;
	}

	if (sign)
	{
		if ( is_operator(target) )
			return;
		_operators.push_back( target );
		this->send_all( target + " is now channel operator" );
	}
	else
	{
		if ( !is_operator(target) && me->get_nickname() != target )
			return;
		_operators.erase( _operators.begin() + find_operator_index( target )) ;
		this->send_all( target + " is no longer operator" );
	}
}

int Channel::is_channelClient( std::string target )
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
		if (_channelClients.at(i) == target)
			return (1);
	return (0);
}

int	Channel::is_operator( std::string target )
{
	for (int i = 0; i < (int)_operators.size(); i++)
		if (_operators.at(i) == target)
			return (1);
	return (0);
}

int	Channel::is_primordial( std::string target )
{
	if (_primordial == target)
		return (1);
	return (0);
}

int	Channel::is_invited( std::string target )
{
	for (int i = 0; i < (int)_invite_list.size(); i++)
		if (_invite_list.at(i) == target)
			return (1);
	return (0);
}

int	Channel::find_client_index( std::string target )
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
	{
		if (_channelClients.at(i) == target)
			return (i);
	}
	return (-1);
}

int	Channel::find_operator_index( std::string target )
{
	for (int i = 0; i < (int)_operators.size(); i++)
	{
		if (_operators.at(i) == target)
			return (i);
	}
	return (-1);
}

int	Channel::find_invited_index( std::string target )
{
	for (int i = 0; i < (int)_invite_list.size(); i++)
	{
		if (_invite_list.at(i) == target)
			return (i);
	}
	return (-1);
}

void	Channel::add_client( std::string user )
{
	_channelClients.push_back( user );
}

void	Channel::kick_client( std::string user )
{
	// if ( is_operator( user ) )
	// 	_operators.erase( _operators.begin() + find_operator_index( user ) );
	if ( is_channelClient( user ) )
		_channelClients.erase( _channelClients.begin() + find_client_index( user ) );
}

void	Channel::remove_from_invite_list( std::string user )
{
	if ( is_invited( user ) )
		_invite_list.erase( _invite_list.begin() + find_invited_index( user ) );
}

void	Channel::send_privmsg_all(std::string source, std::string message )
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
	{
		if (_server->find_client( _channelClients.at(i) )->get_nickname() != source)
			_server->find_client( _channelClients.at(i) )->send_message( message );
	}
}

void	Channel::send_all( std::string message )
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
		_server->find_client( _channelClients.at(i) )->send_message_in_channel( _name ,message );
}

void	Channel::send_all_clients(std::string message)
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
		_server->find_client( _channelClients.at(i) )->send_message(message );
}

void	Channel::send_everyone_else( std::string message, std::string name)
{
	for (int i = 0; i < (int)_channelClients.size(); i++)
	{
		Client *user = _server->find_client( _channelClients.at(i) );
		if (user->get_nickname() != name)
			user->send_message( message );
	}
}

void	Channel::change_nick(std::string oldnick, std::string newnick)
{
	if (find_client_index(oldnick) != -1)
		_channelClients[find_client_index(oldnick)] = newnick;
	if (find_operator_index(oldnick) != -1)
		_operators[find_operator_index(oldnick)] = newnick;
	if (find_invited_index(oldnick) != -1)
		_invite_list[find_invited_index(oldnick)] = newnick;
}