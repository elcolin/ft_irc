/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 20:59:22 by elise             #+#    #+#             */
/*   Updated: 2023/05/01 22:43:16 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Server::Server():port(""), password("")
{
    // std::cerr << "/!\\ Warning no"
    errorin(std::atoi(port) <= 0, "Invalid port.\n");
}

void Server::init_server()
{
    this->listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    errorin(this->listen_socket == -1, strerror(errno));
    flags = fcntl(listen_socket, F_GETFL);
    fcntl(listen_socket, F_SETFL, flags | O_NONBLOCK);
    
    struct addrinfo hints; //defining expected type of address
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    errorin(getaddrinfo(0, port, &hints, &res) != 0, "Failed to get address info.");
    if(bind(this->listen_socket, res->ai_addr, res->ai_addrlen) == -1)
    {
        freeaddrinfo(res);
        errorin(1, "Failed to bind listen socket.");
    }
    freeaddrinfo(res);
    errorin(listen(this->listen_socket, SOMAXCONN) == -1, "Failed to listen on socket.");
}

Server::Server(const char *port, const char *password): port(port), password(password)
{
    errorin(std::atoi(port) <= 0, "Invalid port.\n");
    listen_socket = -1;
    flags = 0;
}

Server::~Server()
{
    if (listen_socket != -1)
        close(listen_socket);
}