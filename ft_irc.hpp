/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:25:08 by elise             #+#    #+#             */
/*   Updated: 2023/05/05 13:18:25 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <iostream>
#include <cstring>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include <unordered_map>
#include <fcntl.h>

void errorin(int err, const std::string msg);
//std::exception??

class Server{
    private:
        const char *port;
        const char *password;
        // int listen_socket;
        int flags;
        struct pollfd sockets[SOMAXCONN + 1];
        unsigned int socket_number;
        unsigned int events_number;
        void init_server();
        int new_connection();
        int exit;
    public:
        Server();
        ~Server();
        Server &operator=(Server const &a);
        Server(const char *port, const char *password);
        void monitoring();
        int shut_down();
};

#endif