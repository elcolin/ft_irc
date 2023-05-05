#include "Client.hpp"







	// Setters

void	set_fd(int fd)
{
	_fd = fd;
}

void	set_username(std::string username)
{
	_username = username;
}

void	set_hostname(std::string hostname)
{
	_hostname = hostname;
}

void	set_nickname(std::string nickname)
{
	_nickname = nickname;
}

	// Getters

int	get_fd()
{
	return _fd;
}

std::string	get_username()
{
	return _username;
}

std::string get_hostname()
{
	return _hostname;
}

std::string 