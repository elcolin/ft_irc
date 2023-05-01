/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:25:08 by elise             #+#    #+#             */
/*   Updated: 2023/05/01 21:27:18 by elise            ###   ########.fr       */
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

void errorin(int err, char *msg);
//std::exception??

class Server{
    private:
        const char *port;
        const char *password;
        int listen_socket;
        int flags;
    public:
        Server();
        ~Server();
        Server &operator=(Server const &a);
        Server(const char *port, const char *password);
        void init_server();
};

#endif