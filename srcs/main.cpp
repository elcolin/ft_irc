/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:43:12 by elise             #+#    #+#             */
/*   Updated: 2023/05/07 16:56:32 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// void errorin(int err, const std::string msg)
// {
//     if (err == 1)
//     {
//         std::cerr << msg << std::endl;
//         exit(EXIT_FAILURE);
//     }
// }
//Code snippet
int main(int argc, char *argv[])
{
    // errorin(argc != 3, "Invalid arguments.");
    if (argc != 3)
    {
        std::cerr << "Invalid arguments.\n";
        exit(EXIT_FAILURE);
    }
    try{
        Server ircserv(argv[1], argv[2]);
        while (!ircserv.shut_down())
            ircserv.monitoring();
    }
    catch(std::exception &e)
    {
        std::cout << "/!\\ "<< e.what();
    }
}
