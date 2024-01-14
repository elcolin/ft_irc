#ifndef	CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include <sys/socket.h>
# include <sys/types.h>
# include <string>
# include <iostream>
# include <sstream>
# include <vector>
# include <deque>
# include "Channel.hpp"
# include "Server.hpp"

class Server;
class Channel;

class	Client
{
	private:
			int 						_fd;
			std::string					_hostname;
			std::string					_username;
			std::string					_realname;
			std::string					_nickname;
			std::vector<std::string>	_channelList;
			std::deque<std::string>	_msg;

			int			_is_registered;

	public:

			Client(int fd, std::string hostname);
			~Client();

		// Setters
		
			void			set_fd(int fd);
			void			set_register(int state);
			void			set_hostname(std::string name);
			void			set_username(std::string name);
			void			set_realname(std::string name);
			void			set_nickname(std::string name);
			void			set_add_channel(Channel* new_channel);

		// Getters

			int							get_fd();
			int							get_registered();	
			std::string					get_hostname();
			std::string					get_username();
			std::string					get_realname();
			std::string					get_nickname();
			std::string					get_fullname();
			std::vector<std::string>	get_channelList();

		// Functions
		void	send_final();
		void	queue_msg(std::string message);
		void	send_reply(std::string message);
		void	send_message(std::string message);
		void	send_privmessage_from(Client *source, std::string message);
		void	send_message_in_channel( std::string channel, std::string message);

};

#endif