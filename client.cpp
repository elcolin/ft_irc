/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 17:20:08 by elise             #+#    #+#             */
/*   Updated: 2023/04/29 17:56:32 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Client::~Client()
{
    
}
Client::Client()
{
    
}
Client::Client(struct sockaddr client_addr, socklen_t client_addr_len): client_addr_len(client_addr_len), client_addr(client_addr)
{
    
}
Client::Client(Client const &a)
{
    
}