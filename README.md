# ft_irc
Internet Relay Chat in C++98

Good references:
http://vidalc.chez.com/lf/socket.html

    Note:
        Need to add back -Wall -Wextra -Werror in makefile

# What is a socket?
## Definition and use
    int socket(int domain, int type, int protocol);
This function creates a communication endpoint, a communication endpoint can be roughly resumed as an identifier bound to an IP address and a port in our case, but it also carries other relevant informations such as the transport layer communication (TCP here).

    A socket is a type of communication endpoint
Having this in mind, to create our endpoint, we need to give him some information of what connections we expect. Based on the given subject here is what I chose:

    socket(AF_INET, SOCK_STREAM, 0);
        //AF_INET       IPv4 Internet protocols
        //SOCK_STREAM   Provides sequenced, reliable, two-way, connection-based
        //              byte streams.  An out-of-band data transmission mechanism
        //              may be supported.
        //0             0 for no specified protocol

Why SOCK_STREAM? It's our socket type, here are some good french explanation regarding the different types: http://vidalc.chez.com/lf/socket.html#twotypes
        
The return value of our function is a file descriptor refering to our freshly created endpoint.
In case of an error, it'll return -1

## My first socket
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
 This is our listening socket, it'll be listening for incoming connections and new client connections.

# struct addrinfo / getaddrinfo
We are going to see the use of getaddrinfo, who's objective is to give us a list of addresses, but we need to make sure it delivers only compatible addresses, here how to do so.

## How can I tell getaddrinfo I only want specific addresses?
First we need to create an addrinfo structure:

    struct addrinfo hints;
This structure contains the following members:

    struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
    };
 We don't have to set each members, there are only three parameters that we're interested to set in our case:
 
    int ai_flags;
    int ai_family;
    int ai_socktype;
    
 as following:
 
    hints.ai_family = AF_INET;//IPv4 connections
    hints.ai_socktype = SOCK_STREAM;//connection based flux
    hints.ai_flags = AI_PASSIVE;//see below
    
The two first parameters are just like the ones we gave to the socket, and AI_PASSIVE indicates that the socket address structure will be used for binding a socket that will accept incoming connections.
## Get my addr info
    int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
Quick description of what you're looking at:

    const char *node
    takes an internet hostname or IP address
    
    const char *service
    specifies the server or port to connect to
    
    const struct addrinfo *hints
    takes the address of an addrinfo structure with the parameters desired for our futur address list
    
    struct addrinfo **res
    our wanted address list!
The function will return 0 on success, other on failure, make sure to handle it.
What we get finally:

     struct addrinfo *res;//to store our list
     getaddrinfo(0, port, &hints, &res);
Now that we have the informations we need, we can bind our socket!

# bind
