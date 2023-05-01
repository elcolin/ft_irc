/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:22:32 by elise             #+#    #+#             */
/*   Updated: 2023/05/01 10:52:16 by elise            ###   ########.fr       */
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
    int flags = fcntl(listen_socket, F_GETFL, 0);
    fcntl(listen_socket, F_SETFL, flags | O_NONBLOCK);
    struct pollfd fds[SOMAXCONN + 1] = {0};
    fds[0].fd = listen_socket;
    fds[0].events = POLL_IN;
    size_t i = 1;
    while (true)
    {
        int num_events = poll(fds, i, -1);
        if (num_events == -1)
        {
            std::cerr << " Failed poll() execution.\n";
            continue;
        }
        for (int j = 0; j < i; j++)
        {
            if (fds[j].revents == POLL_ERR)
                std::cerr << "/!\\ Warning: An error occurred on a file descriptor.\n";
            if (fds[j].revents != POLLIN)
                continue;
            fds[j].revents = 0;
            if (fds[j].fd == listen_socket)
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);

                fds[i].fd = accept(listen_socket, (sockaddr *) &client_addr, &client_addr_len);
                if (fds[i].fd == -1)
                {
                    std::cerr << "Failed to accept incoming connection.\n";
                    fds[i].fd = 0;
                    continue;
                }
                std::cout << "New connection successfull!\n";
                fds[i++].events = POLL_IN;
            }
            else
            {
                char buffer[1024];
                int bytes_received = recv(fds[j].fd , buffer, sizeof buffer, 0);
                buffer[bytes_received] = '\0';
                std::cout << buffer;
            }

        }
    }
}