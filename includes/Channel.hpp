#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Replies.hpp"
# include "Server.hpp"
# include <vector>

class Server;
class Client;

class Channel
{
	private:

		std::string					_name;
		std::string					_topic;
		std::vector<std::string>	_channelClients;	// Tout les client du channel sont dans _channelClient;
		std::vector<std::string>	_operators;			// Les modo dont dans _channelClient && _operators;
		std::string					_primordial;		// Le primordial est dans les 3
		Server						*_server;
		std::vector<std::string>	_invite_list;


// MODE
		bool						_invite_only;			//set to 0 when init
		bool						_channel_key; 			//set to 0 when init
		std::string					_password;
		bool						_restriction_TOPIC_cmd;	//set to 0 when init
		bool						_user_limit; 			//set to 0 when init
		int							_user_limit_nb;

	public:

// CONSTRUCTOR
		Channel(std::string name, std::string primordial, Server &server);
		~Channel();

// SETTER
		void						set_invite_only( int i );
		void						set_channel_key( int i, std::string password );
		void						set_user_limit( int limit );
		void						set_restriction_TOPIC_cmd( int i );
		void						set_topic( std::string name );
		void						set_primo( std::string new_primo );
		void						add_operator( std::string client );
		void						add_invite( std::string invite );

// GETTER

		std::string					get_name();
		Server						*get_server();
		std::vector<std::string>	get_channelClients();
		std::vector<std::string>	get_operators();
		std::string					get_topic();
		std::vector<std::string>	get_invite();

		bool						get_invite_only();
		bool						get_channel_key();
		std::string					get_password();
		bool						get_user_limit();
		int							get_user_limit_nb();
		bool						get_restriction_TOPIC_cmd();

// FUNCTION

		void				operator_privilege( Client *me, std::string target, int sign );

		int					is_primordial( std::string target );
		int					is_operator( std::string target );
		int					is_channelClient( std::string target );
		int					is_invited( std::string target );
		void				remove_from_invite_list( std::string user );

		int					find_client_index( std::string target );
		int					find_operator_index( std::string target );
		int					find_invited_index( std::string target );

		void				change_nick(std::string oldnick, std::string newnick);
		void				add_client( std::string user );
		void				kick_client( std::string user );
		void				send_all( std::string message );
		void				send_everyone_else( std::string message, std::string name);
		void				send_all_clients(std::string message);
		void				send_privmsg_all( std::string	source, std::string message );


};

#endif