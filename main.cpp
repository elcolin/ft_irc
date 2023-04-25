/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:22:32 by elise             #+#    #+#             */
/*   Updated: 2023/04/25 16:02:53 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void errorin(int err, const char *msg)
{
    if (err == 1)
    {
        std::cerr << msg << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    errorin(argc != 3, "Invalid arguments.");
    
    const char* port = argv[1];
    const char* password = argv[2];
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    errorin(listen_socket == -1, "Failed to create socket.");

    struct addrinfo hints; //defining expected type of address
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    errorin(getaddrinfo(0, port, &hints, &res) != 0, "Failed to get address info.");
    errorin(bind(listen_socket, res->ai_addr, res->ai_addrlen) == -1, "Failed to bind listen socket.");
    freeaddrinfo(res);
    errorin(listen(listen_socket, SOMAXCONN) == -1, "Failed to listen on socket.");
    std::cout << "Listening on port " << port << "..." << std::endl;
    // struct sockaddr_in *addr_in = (struct sockaddr_in*)(res->ai_addr);
    // std::cout << "IP address: " << inet_ntoa(addr_in->sin_addr) << std::endl;
}