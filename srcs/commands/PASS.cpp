#include "Replies.hpp"
#include "Server.hpp"

// 		/PASS <password>

// PASS ne requiert pas du client d'etre authentifie
// 

void    Pass(Client *client, std::vector<std::string> args, Server &serv)
{
    if (args.size() < 2)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_hostname(), "PASS"));
        return ;
    }
    if (check_auth(client))
    {
        client->send_reply(ERR_ALREADYREGISTERED(client->get_nickname()));
        return ;
    }
    if (args[1].compare(serv.get_password()) != 0)
    {
    	client->send_reply(ERR_PASSWDMISMATCH(client->get_hostname()));
        return ;
    }
	else
		client->set_register(PASS_CHECKED);
}