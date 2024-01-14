#ifndef	SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"
#include "Replies.hpp"
#include <sstream>
#include <iostream>
#include <cstring>
#include <sys/types.h> 
#include <sys/socket.h>				 
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/poll.h>
#include <errno.h>
#include <list>
#include "Color.h"
#include "Commands.hpp"
#include <map>
#include <signal.h> 

# define MAX_CONN 100

extern int				 exit_state;

class	Client;
class   Channel;

class Server
{
    private:

        const char					*_port;
        const char					*_password;
		std::map<int, Client *>		_clientList;
		std::vector<pollfd>			_sockets;
		int 						_listening_socket;

        int								flags;
        struct pollfd					sockets[SOMAXCONN]; //listening socket is sockets[0]
        int								socket_number;
        int								events_number;
        void							init_server();
        std::vector<pollfd>::iterator	new_connection();
        std::vector<pollfd>::iterator	disconnect(int fd);
		
        std::vector<Channel>			_Channels;
	
    public:

        Server();
        ~Server();
        Server &operator=(Server const &a);
        Server(const char *port, const char *password);

		void	monitoring();
		int 	shut_down();
		void	adduser(int fd, std::string hostname);
		void	command_handler(Client *client, std::vector<std::string> args);
		void	sig_handler(int);
		bool	handle_data(std::vector<pollfd>::iterator it);
		void	errorin(bool err, const char *msg);

		Client	*find_client(std::string nickname);
		Channel	*find_channel(std::string channel_name);
		void	del_channel( std::string channel_name );
		void	quit_channels( Client *client, Server &serv );

		void	broadcast_server(std::string message);

	// SETTER

		void					add_Channels(Channel channel);

	// GETTER

		std::vector<Channel>	get_Channels();
		
	// EXCEPTION
	
		class SocketException : public std::exception
		{
			private:
				const char  *msg;
			public:
				SocketException(const char *msg) : msg(msg) {}
				virtual const char* what() const throw()
				{
					return msg;
				}
		};
		std::string				get_password();
};

#endif