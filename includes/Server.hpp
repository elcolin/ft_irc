#ifndef	SERVER_HPP
# define SERVER_HPP

// # include "Client.hpp"
// # include "Channel.hpp"
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

void errorin(int err, const std::string msg);

class Server
{
    private:
        const char		*port;
        const char		*password;
        int				flags;
        struct pollfd	sockets[SOMAXCONN + 1]; //listening socket is sockets[0]
        int	socket_number;
        int	events_number;
		int				exit;
        void			init_server();
        int				new_connection();
	
    public:

        Server();
        ~Server();
        Server &operator=(Server const &a);
        Server(const char *port, const char *password);
        void monitoring();
        int shut_down();
};

// class	Server
// {
// 	private:
// 			std::string					_password;
// 			std::string					_hostname;
// 			int							_port;
// 			std::map<int, Client *>		_clientList;
// 			Channel						*_channels;

// 	public:

// 			Server(int port, std::string password);
// 			~Server();
// };

#endif