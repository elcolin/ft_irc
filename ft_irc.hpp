/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:25:08 by elise             #+#    #+#             */
/*   Updated: 2023/04/29 23:38:06 by elise            ###   ########.fr       */
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

class Client{
    private:
        socklen_t client_addr_len;
        struct sockaddr_in client_addr;
        int sockfd;
    public:
    //create constructor
    ~Client();
    Client();
    Client(struct sockaddr_in client_addr, socklen_t client_addr_len, int sockfd);
    Client(Client const &a);
    Client &operator=(Client const &a);
    
};

#endif