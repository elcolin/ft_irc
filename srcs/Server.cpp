/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:45:06 by elise             #+#    #+#             */
/*   Updated: 2023/05/09 16:30:44 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server():port(""), password("")
{
    errorin(std::atoi(port) <= 0, "Invalid port.");
}

bool Server::shut_down()
{
    if (exit)
        return (true);
    return (false);
}

void Server::errorin(bool err, const char *msg)
{
    if (err == true)
        throw SocketException(msg);
}

int Server::new_connection()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    sockets[socket_number].fd = accept(sockets[0].fd, (sockaddr *) &client_addr, &client_addr_len);
    if (sockets[socket_number].fd == -1)
    {
        std::cerr << "Failed to accept incoming connection.\n";
        sockets[socket_number].fd = 0;
        return (-1);
    }
    std::cout << "New connection successfull!\n";
    sockets[socket_number++].events = POLL_IN;
    return (0);
}

void Server::disconnection(int client_socket)
{
    bool found = false;
    std::cout << "A client has disconnected.\n";//print addr?
    for (int i = 0; i < socket_number; i++)
    {
        if (sockets[i].fd == client_socket)
        {
            socket_number--;
            errorin(close(sockets[i].fd) == -1, strerror(errno));
            found = true;
        }
        if (found)
        {
            sockets[i] = sockets[i + 1];
            memset(&sockets[i + 1], 0, sizeof sockets[i + 1]);
        }
    }
}

void Server::monitoring()
{
    char buffer[1024];
    int bytes_received = 0;
    events_number = poll(sockets, socket_number, 1000);
    
    if (events_number == -1)
    {
        std::cerr << " Failed poll() execution.\n";
        exit = true;
        return;
    }
    for (int j = 0; j < socket_number; j++)
    {
        if (sockets[j].revents & POLL_ERR)
            std::cerr << "/!\\ Warning: An error occurred on a file descriptor.\n";
        if (sockets[j].revents & POLLHUP)
            disconnection(sockets[j].fd);
        if (sockets[j].revents != POLLIN)
        { 
            sockets[j].revents = 0;
            continue;
        }
        sockets[j].revents = 0;
        if (sockets[j].fd == sockets[0].fd)
            new_connection();
        else //handle message function needed, below is just a snippet
        {
            bytes_received = recv(sockets[j].fd , buffer, sizeof buffer, 0);
            buffer[bytes_received] = '\0';
            std::cout << buffer;
            if (!strncmp(buffer, "SHUTDOWN", 8))// temporary closing solution for server
                exit = true;
        }

    }
}

void Server::init_server()
{
    sockets[0].fd = socket(AF_INET, SOCK_STREAM, 0);//listen socket
    errorin(sockets[0].fd == -1, strerror(errno));
    flags = fcntl(sockets[0].fd, F_GETFL);
    fcntl(sockets[0].fd, F_SETFL, flags | O_NONBLOCK);//non blocking flags set
    
    struct addrinfo hints; //defining expected type of address
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    errorin(getaddrinfo(0, port, &hints, &res) != 0, "Failed to get address info.\n");
    if(bind(sockets[0].fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        freeaddrinfo(res);
        errorin(1, "Failed to bind listen socket.\n");
    }
    freeaddrinfo(res);
    errorin(listen(sockets[0].fd, SOMAXCONN) == -1, "Failed to listen on socket.\n");
    sockets[0].events = POLL_IN;
    socket_number = 1;
    std::cout << "Listening on port " << port << "..." << std::endl;
}

Server::Server(const char *port, const char *password): port(port), password(password)
{
    errorin(std::atoi(port) <= 0, "Invalid port.\n");
    memset(&sockets, 0, SOMAXCONN + 1);
    exit = false;
    flags = 0;
    init_server();
}

Server::~Server()
{
    fcntl(sockets[0].fd, F_SETFL, O_RDONLY);
    // std::cout << socket_number << "\n";
    for (int i = socket_number - 1; i >= 0; i--)
    {
        if (sockets[i].fd)
        {
            errorin(close(sockets[i].fd) == -1, strerror(errno));
            sockets[i].fd = 0;
        }
    }
    std::cout << "Server shutted down successfully" << std::endl; 
}
