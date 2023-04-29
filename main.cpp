/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:22:32 by elise             #+#    #+#             */
/*   Updated: 2023/04/29 17:18:30 by elise            ###   ########.fr       */
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
    errorin(bind(listen_socket, res->ai_addr, res->ai_addrlen) == -1, "Failed to bind listen socket.");//associates socket with local port
    freeaddrinfo(res);
    errorin(listen(listen_socket, SOMAXCONN) == -1, "Failed to listen on socket.");
    std::cout << "Listening on port " << port << "..." << std::endl;
    std::map<int, struct sockaddr> client;
    while (true)
    {
        struct sockaddr client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(listen_socket, &client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            std::cerr << "Failed to accept incoming connection.\n";
            continue;
        }
        std::cout << "New connection successfull!\n";
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive data from client." << std::endl;
            close(client_socket);
            continue;
        }
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';
            std::cout << buffer;
        } 
    }
    // char buffer[1024];
    // int bytes_received = recv(client_socket, buffer, sizeof buffer, 0);
        // if (bytes_received == -1) {
            // std::cerr << "Failed to receive data from client." << std::endl;
            // close(client_socket);
            // continue;
        // }
    // struct sockaddr_in *addr_in = (struct sockaddr_in*)(res->ai_addr);
    // std::cout << "IP address: " << inet_ntoa(addr_in->sin_addr) << std::endl;
}