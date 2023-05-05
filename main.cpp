/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 12:22:32 by elise             #+#    #+#             */
/*   Updated: 2023/05/05 13:34:16 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void errorin(int err, const std::string msg)
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
    
    Server ircserv(argv[1], argv[2]);
    while (!ircserv.shut_down())
        ircserv.monitoring();
}