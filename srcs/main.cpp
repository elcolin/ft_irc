/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elise <elise@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 13:43:12 by elise             #+#    #+#             */
/*   Updated: 2023/05/26 11:12:05 by elise            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	sig_handler(int)
{
	throw	std::runtime_error("Program killed");
	// exit_state = 1;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Invalid arguments.\n";
        exit(EXIT_FAILURE);
    }
    try{
        Server ircserv(argv[1], argv[2]);
        signal(SIGINT, &sig_handler);
        std::cout << "\n\t-- Server initialized on port " << argv[1] << " --\n" << std::endl; 
        while (!ircserv.shut_down())
            ircserv.monitoring();
    }
    catch(std::exception &e)
    {
        std::cout << RED << "/!\\ "<< e.what() << END<<std::endl;
    }
}
