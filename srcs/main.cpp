/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:43:12 by elise             #+#    #+#             */
/*   Updated: 2023/05/07 17:09:55 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//Code snippet
int main(int argc, char *argv[])
{
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
