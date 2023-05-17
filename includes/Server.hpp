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

class Server
{
    private:
        const char		*port;
        const char		*password;
        int				flags;
        struct pollfd	sockets[SOMAXCONN]; //listening socket is sockets[0]
        int             socket_number;
        int             events_number;
		bool			exit;
        void			init_server();
        int				new_connection();
        void            disconnection(int client_socket);
	
    public:
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
        Server();
        ~Server();
        Server &operator=(Server const &a);
        Server(const char *port, const char *password);
        void monitoring();
        bool shut_down();
        void errorin(bool err, const char *msg);
};

#endif