# ft_irc
Internet Relay Chat in C++98

Good references:
http://vidalc.chez.com/lf/socket.html

    Note:
        Need to add back -Wall -Wextra -Werror in makefile

# What is a socket?
    socket(int domain, int type, int protocol)
This function creates a communication endpoint, a communication endpoint can be roughly resumed as an identifier bound to an IP address and a port in our case, but it also carries other relevant informations such as the transport layer communication (TCP here).

    A socket is a type of communication endpoint
Having this in mind, to create our endpoint, we need to give him some information of what connections we expect. Based on the given subject here is what I chose:

    socket(AF_INET, SOCK_STREAM, 0);
        //AF_INET       IPv4 Internet protocols
        //SOCK_STREAM   Provides sequenced, reliable, two-way, connection-based
        //              byte streams.  An out-of-band data transmission mechanism
        //              may be supported.
        //0             0 for no specified protocol

Why SOCK_STREAM? Good french explanation right here ---> http://vidalc.chez.com/lf/socket.html#twotypes
        
The return value of our function is a file descriptor refering to our freshly created endpoint.
