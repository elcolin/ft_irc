#ifndef	CLIENT_HPP
# define CLIENT_HPP

# include <sys/socket.h>


class	Client
{
	private:
			int fd;
			std::string	_username;
			std::string	_nickname;
			std::string	_hostname;
	public:

			Client(int fd, std::string hostname);
			~Client();

		// Setters
			void	set_fd(int fd);
			void	set_username(std::string name);
			void	set_nickname(std::string name);
			void	set_hostname(std::string name);

		// Getters
			int				get_fd();
			std::string		get_username();
			std::string		get_nickname();
			std::string		get_hostname();
};

#endif