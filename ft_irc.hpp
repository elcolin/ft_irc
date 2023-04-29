/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:25:08 by elise             #+#    #+#             */
/*   Updated: 2023/04/29 18:08:53 by elise            ###   ########.fr       */
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
#include <map>

void errorin(int err, char *msg);
//std::exception??

class Client{
    private:
        socklen_t client_addr_len;
        struct sockaddr client_addr;
    public:
    //create constructor
    ~Client();
    Client();
    Client(struct sockaddr client_addr, socklen_t client_addr_len);
    Client(Client const &a);
    &operator=(Client const &a);
    
};

#endif