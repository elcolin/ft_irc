#include "Server.hpp"

int exit_state = 0;

void Server::errorin(bool err, const char *msg)
{
    if (err == true)
        throw SocketException(msg);
}


Server::Server():_port(""), _password("")
{
    errorin(std::atoi(_port) <= 0, "Invalid port.");
}

int Server::shut_down()
{
    if (exit_state)
        return (1);
    return (0);
}

void	Server::command_handler(Client *client, std::vector<std::string> args)
{
	std::string		command_names[14] = {"USER", "userhost", "PASS", "QUIT", "NICK", "PRIVMSG", "PING", "TOPIC", "JOIN", "MODE", "PART", "KICK", "INVITE", "CAP"};
	void	(*command_functions[14])(Client *, std::vector<std::string>, Server &) = {&User, &User, &Pass, &Quit, &Nick, &Privmsg, &Ping, &Topic, &Join, &Mode, &Part, &kick, &Invite, &Cap};

	for (int i = 0; i < 14; i++)
	{
		if (args[0] == command_names[i])
			command_functions[i](client, args, *this);
	}
}

bool Server::handle_data(std::vector<pollfd>::iterator it)
{
	char buffer[1024];
	int bytes_received = recv(it->fd , buffer, sizeof(buffer), 0);
	buffer[bytes_received] = '\0';

	if (!bytes_received || bytes_received < 0) //tentative de resolution de ctrl-c
		return (false);
	std::cout << GREEN << "\t[RECEIVED]"<<END<<std::endl;
	std::cout << buffer << std::endl;

	std::string			tmp_buffer(buffer);
	std::stringstream	ss(tmp_buffer);
	std::string line;

	while (std::getline(ss, line))
	{
		std::vector<std::string>	received = parse(line);
		if (!received.empty())
			command_handler(_clientList[it->fd], received);
	}
	return (true);
}

std::vector<pollfd>::iterator Server::disconnect(int fd)
{
	//TODO: lui faire quitter son channel actif s'il en a un
	quit_channels(_clientList[fd], *this);
	std::vector<pollfd>::iterator it =_sockets.begin();
	while(it != _sockets.end())
	{
		if (it->fd == fd)
		{
			if (_clientList[it->fd]->get_nickname() != "")
				std::cout << MAGENTA <<"\t"<< _clientList[it->fd]->get_nickname() << " has disconnected\n" << END << std::endl;
			else
				std::cout << MAGENTA << "\tRegistration failed, user disconnected\n" << END << std::endl;
			it = _sockets.erase(it);
			delete(_clientList.at(fd));
			_clientList.erase(fd);
			close(fd);
			return it;
		}
		it++;
	}
	return _sockets.begin();
}

std::vector<pollfd>::iterator Server::new_connection()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
	int fd = accept(_listening_socket, (sockaddr *)&client_addr, &client_addr_len);
	if (fd == -1)
	{
		std::cerr << RED << "/!\\ Failed to accept incoming connection. " << END << std::endl;
		return _sockets.begin();
	}
	pollfd	new_poll = {fd, POLLIN | POLLOUT, 0};
	_sockets.push_back(new_poll);
	adduser(fd, inet_ntoa(client_addr.sin_addr));
    std::cout << MAGENTA << "\tNew connection successfull!" << END << std::endl;
	std::cout<<std::endl;
	return _sockets.begin();
}


void Server::monitoring()
{
	errorin(poll(&(*_sockets.begin()), _sockets.size(), 1000) == -1, " Failed poll() execution.\n");
	std::vector<pollfd>::iterator it = _sockets.begin();
	if (it->revents & POLLIN && _sockets.size() < MAX_CONN + 1) //nouvelle requete
	{
		it = new_connection();
		if (_sockets.size() == MAX_CONN)
			std::cerr << RED << "/!\\ Warning: Max connections reached: " << MAX_CONN << END << std::endl;
	}
	it->revents = 0;
	it++;
	while(it != _sockets.end())
	{
		short revents = it->revents;
		it->revents = 0;
		if (revents & POLLHUP || revents & POLLERR) //deco de _clienList[it->fd]
		{
			if (revents & POLLERR)
				std::cerr << YELLOW << "Interrupted connection with file descriptor." << END << std::endl;
			it = disconnect(it->fd);
		}
		else
		{
			if (revents & POLLOUT)
				_clientList[it->fd]->send_final();
			if (revents & POLLIN)
			{
				if (!handle_data(it))
					it = disconnect(it->fd);
				else if (_clientList[it->fd]->get_registered() == NOT_REGISTERED || _clientList[it->fd]->get_registered() == DISCONNECTED)
					it = disconnect(it->fd);
			}
			else
				it++;
		}
	}
} 

void Server::init_server()
{
    _listening_socket = socket(AF_INET, SOCK_STREAM, 0);//listen socket
    errorin(_listening_socket == -1, strerror(errno));
	int optval = 1;
	setsockopt(_listening_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    fcntl(_listening_socket, F_SETFL, O_NONBLOCK);//non blocking flags set
	struct	sockaddr_in	serv = {};
    std::memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(atoi(_port));
	serv.sin_addr.s_addr = INADDR_ANY; // permet de se bind a n'importe quelle IP
    errorin(bind(_listening_socket, (struct sockaddr *)&serv, sizeof(serv))  == -1, "Failed to bind listen socket.");
    errorin(listen(_listening_socket, SOMAXCONN) == -1, "Failed to listen on socket.");
	pollfd	first_socket = {_listening_socket, POLLIN, 0};
	_sockets.push_back(first_socket);
}

Server::Server(const char *port, const char *password): _port(port), _password(password)
{
    errorin(std::atoi(_port) <= 0, "Invalid port.\n");
    init_server();
}

Server::~Server()
{
	if (!_clientList.empty())
		for (std::map<int, Client*>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
			delete(it->second);
    for (std::vector<pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
    {
        if (it->fd)
        {
            if (close(it->fd))
                std::cerr << RED << "/!\\ Error while closing file descriptor: " << strerror(errno) << END << std::endl;
            it->fd = 0;
        }
    }
	exit_state = 1;
    std::cout << "\n\t-- Server shutted down successfully --\n" << std::endl; 
}

void	Server::adduser(int fd, std::string hostname)
{
	Client *newuser = new Client(fd, hostname);
	_clientList.insert(std::make_pair(fd, newuser));
}

void	Server::quit_channels( Client *client, Server &serv )
{
	std::vector<std::string> channel_list = client->get_channelList();

	for ( int i = 0; i < (int)channel_list.size(); i++ )
	{
		Channel *channel = serv.find_channel( channel_list[i] );

		channel->kick_client( client->get_nickname() );
		// for (int o = 0; o < (int)channel->get_channelClients().size(); o++ )
		// 	std::cout << channel->get_channelClients()[o] << std::endl;
		// for (int o = 0; o < (int)channel->get_operators().size(); o++ )
		// 	std::cout << channel->get_operators()[o] << std::endl;
	}
}

void	Server::broadcast_server(std::string message)
{
	for (std::map<int, Client *>::iterator it = _clientList.begin(); it != _clientList.end(); it ++)
		it->second->send_message(message);
}



//****************************************************//
//              Channel/Client Function               //
//****************************************************//

Client  *Server::find_client(std::string nickname)
{
	for ( std::map<int, Client*>::iterator it = _clientList.begin(); it != _clientList.end(); it++ )
		if ( nickname == it->second->get_nickname() )
			return ( it->second );
	return (0);
}

Channel *Server::find_channel( std::string channel_name )
{
	for ( int i = 0; i < (int)_Channels.size(); i++ )
	{
		if ( channel_name == _Channels.at(i).get_name() )
			return ( &_Channels.at(i) );
	}
	return (0);
}

void	Server::del_channel( std::string channel_name )
{
	for ( int i = 0; i < (int)_Channels.size(); i++ )
	{
		if ( channel_name == _Channels.at(i).get_name() )
			_Channels.erase( _Channels.begin() + i );
	}
}

//****************************************************//
//                      Getter                        //
//****************************************************//

std::vector<Channel>	Server::get_Channels()
{
	return (_Channels);
}

std::string	Server::get_password()
{
	return (_password);
}

//****************************************************//
//                      Setter                        //
//****************************************************//

void	Server::add_Channels(Channel new_channel)
{
	_Channels.push_back(new_channel);
}
