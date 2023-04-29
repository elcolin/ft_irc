/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 17:20:08 by elise             #+#    #+#             */
/*   Updated: 2023/04/29 18:26:40 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Client::~Client()
{
    
}
Client::Client()
{
    
}
Client::Client(struct sockaddr_in client_addr, socklen_t client_addr_len, int sockfd): client_addr_len(client_addr_len), client_addr(client_addr), sockfd(sockfd)
{
    
}
Client::Client(Client const &a)
{
    
}

Client &Client::operator=(Client const &a)
{
    this->client_addr_len = a.client_addr_len;
    this->client_addr = a.client_addr;
    this->sockfd = a.sockfd;
    return *this;
}