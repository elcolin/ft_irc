# ft_irc
## Internet Relay Chat in C++98
The project aims to create a server IRC that can be launched with this command:
    
    ./ircserver <port> <password>
    
and whom you can connect to through an IRC client of your choice

Good references:
http://vidalc.chez.com/lf/socket.html

    Note:
        Need to add back -Wall -Wextra -Werror in makefile
        finish canonical form in client.cpp
 
# Creating a server
First step is the creating of our server, who needs to be able to receive and treat connections properly, let's dive into our first mandatory element.
## Sockets
https://man7.org/linux/man-pages/man2/socket.2.html
### What is a socket?
Sockets allow programs to create, send, receive, and manage data across a network, making it possible for applications to communicate with each other over a network. They provide a way for programs to communicate by establishing connections and sending data between them. They do so by creating a communication endpoint.

    int socket(int domain, int type, int protocol);
This function creates a communication endpoint, a communication endpoint can be roughly resumed as an identifier bound to an IP address and a port in our case, but it also carries other relevant informations such as the transport layer communication (TCP here).

    A socket is a type of communication endpoint
### How to use the socket function?
Let's take a look:

    int domain    protocol family, such as IP
    int type      the type of socket you wish to create, choose that depending on the transmetted information
    int protocol  the protocol you want to use, set to 0 if you want it to be chose by your operating system
Having this in mind, to create our endpoint, we need to give him some information on what connections we expect. Based on the given subject here is what I chose:

    socket(AF_INET, SOCK_STREAM, 0);
        //AF_INET       IPv4 Internet protocols
        //SOCK_STREAM   Provides sequenced, reliable, two-way, connection-based
        //              byte streams.  An out-of-band data transmission mechanism
        //              may be supported.
        //0             0 for no specified protocol

Why SOCK_STREAM? It's our socket type, here are some good french explanation regarding the different types: http://vidalc.chez.com/lf/socket.html#twotypes
        
The return value of our function is a file descriptor refering to our freshly created endpoint.
In case of an error, it'll return -1

### My first socket
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
This is our listening socket, it'll be listening for incoming connections and new client connections.
We're not finished setting our socket, since we need to bound it to an address (and port) in order to be able to access it over the network.

## struct addrinfo / getaddrinfo
http://manpagesfr.free.fr/man/man3/getaddrinfo.3.html

We are going to see the use of getaddrinfo, who's objective is to give us a list of addresses that we can bind to our socket (or else it'll just take a random one on the system), but we need to make sure it delivers only compatible addresses, here's how to do so.

### How can I tell getaddrinfo I only want certain types of addresses?
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
    int ai_family; protocol family
    int ai_socktype; our socket type
    
 as following:
 
    hints.ai_family = AF_INET;//IPv4 connections
    hints.ai_socktype = SOCK_STREAM;//connection based flux
    hints.ai_flags = AI_PASSIVE;//see below
    
The two first parameters are just like the ones we gave to the socket, and AI_PASSIVE indicates that the socket address structure will be used for binding a socket that will accept incoming connections.
### Get my addr info
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
The function will return 0 on success, other then 0 on failure, make sure to handle it.
What we get finally:

     struct addrinfo *res;//to store our list
     getaddrinfo(0, port, &hints, &res);// our node might change later, for now we stay in localhost
Now that we have the informations we need, we can bind our socket!

## bind
https://man7.org/linux/man-pages/man2/bind.2.html
### What is bind?
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    
bind() associates a socket with a specific address and port number, which means that it assigns the local address to the socket. This is necessary for the socket to be able to receive incoming data packets from other networked devices. Once a socket is bound to an address and port, it is ready to accept incoming connections from clients.

### Why do we have to bind our socket to an address?
Binding an address to a socket is necessary to specify which network interface and port the socket will be listening on. Without binding an address to the socket, the operating system will choose a random port and network interface for the socket to listen on, which may not be what we intended. By specifying a specific address to bind to, we can ensure that the socket will be listening on the correct network interface and port.

So with getaddrinfo, we got our list of suitable addresses for our service (port).
Let's look at our parameters:
    
    int sockfd --> our file descriptor that refers to the socket.
    const struct sockaddr *addr --> the address we want to bind our socket to
    socklen_t addrlen --> the size of the address structure

We get the following:

    bind(listen_socket, res->ai_addr, res->ai_addrlen);
    //res->ai_addr points to a sockaddr structure that represents the network address of the server, in the format specified by the ai_family
Now we can start to listen for incoming connections!

## Hey listen!
http://manpagesfr.free.fr/man/man2/listen.2.html

    int listen(int sockfd, int backlog);
### What's the purpose of listen?
We don't want to connect to a distant host, we instead want to wait for incoming client connections, here is where the listen function comes along.
It allows to set the socket in a "passive" state, meaning it's waiting for a client to connect.

    int sockfd --> our listening socket
    int backlog --> the maximum number of allowed incoming connections that can be queued of the given socket

We'll give the following parameters:

    listen(listen_socket, SOMAXCONN);
    //listen_socket is our previously created socket
    //SOMAXCONN is a constant defined in the socket.h header file. It represents the maximum length to which the queue of pending connections for this         //socket may grow.
Now our listening socket is fully operational, we can concentrate on client connections.

## Accept 
https://man7.org/linux/man-pages/man2/accept.2.html
Accepts a connection on a socket

    int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);

### What does accept do? 
The accept function blocks the server until a client connect's to the listening socket. When accepted, it returns a new file descriptor for the new socket. It will create a new socket for each new successfull connections, allowing simultaneous communication with multiple clients.

### How does accept work ?
As you can see above, and especially for the second parameter, the function requires a structure: This new structure is needed to store the address of the client trying to connect.

    int sockfd --> the listening socket
    struct sockaddr *restrict addr --> where the client's address will be stored
    socklen_t *restrict addrlen --> as an argument will be passed the size of the structure, but will contain the actual size of the address on return
    
returns -1 if an error occurs

So we create a new structure and initialize our length for the client:

    struct sockaddr client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

Then we can pass it to accept:
    
    int client_socket = accept(listen_socket, &client_addr, &client_addr_len);
    
If you want to repeat this operation (for multiple connections as required), you must include it in a while(true) loop.
## Our running server
A peak of how it should look like:
    
    char *port = argv[1];
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *res;
    getaddrinfo(0, port, &hints, &res);
    bind(listen_socket, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    listen(listen_socket, SOMAXCONN);
    std::cout << "Listening on port " << port << "..." << std::endl;
    while (true)
    {
        struct sockaddr client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(listen_socket, &client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            std::cerr << "Failed to accept incoming connection.\n";
            continue;
        }
        std::cout << "New connection successfull!\n";
    }
    
*You can't take it under this form in your code: every new connection will replace the older file descriptor, but it is enough to check that your program is working, you should use an array instead.*

Keep in mind that all the error handling has been taken out for lisibility, make sure to think about it!
Now if you do:

    ./ircserver <a port>

You'll have a running server pending for an incoming connection on your port! That means you can locally try it out:

    Go on any IRC client (I use Lime Chat), and enter 127.0.0.1 and the port you chose, it should say "Connected"!
### Accessing it over the network
All you need to do is
    
    ifconfig

Or ipconfig depending on the machine where you run the server. Run it in your terminal, then find the IP under this format:

    inet X.X.X.X netmask 0xffffff00 broadcast X.X.X.X
 
That is not 127.0.0.1, what is right after inet is your IP address.
Meaning you can go on any machine on your wifi, launch your IRC client with this IP and the correct port and be able to connect to your server.

### What next?
We can connect to our server locally and over the network, but that's it. Let's see how we can receive and treat data.

# Receiving data
How do we know if we're receiving data from our clients? Let's dig into the poll() function.

## Poll
http://manpagesfr.free.fr/man/man2/poll.2.html
We're going to review a mandatory function, which is poll():

    int poll(struct pollfd *fds, nfds_t nfds, int delay);
    
 Poll is used to monitor file descriptors, waiting for an event, such as incoming data. It requires a pollfd structure.
 
 ### struct pollfd
 
    struct pollfd {
        int   fd;         /* file descriptor */
        short events;     /* expected event    */
        short revents;    /* detected event    */
    };
 
When an event occurs on one of the monitored file descriptors, poll() returns, and you can examine the revents field of each struct pollfd to determine which events occurred on which descriptors.

Let's look at poll's arguments:
    
    struct pollfd   *fds    our structure filled with file descriptors (sockets in our case) and expected event
    nfds_t          nfds    the number of file descriptor to monitor
    int             delay   specifies the amount of time in milliseconds that poll will block for waiting for events to occur on any of the fd
    
We need an array of our opened file descriptors, the client sockets that are returned from accept (remember when I said the right way would be to do an array? There it is). I personnally did this:

    struct pollfd fds[SOMAXCONN + 1] = {0};
    
Since we cannot have more SOMAXCONN connections plus one for our listening socket. It would be a shame to not monitor our listening socket for incoming connections.
    
    int i = 1;
    struct pollfd fds[SOMAXCONN + 1] = {0};
    fds[0] = listen_socket;
    while (true)
    {
        struct sockaddr client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int client_socket = accept(listen_socket, &client_addr, &client_addr_len);
        if (client_socket == -1)
        {
            std::cerr << "Failed to accept incoming connection.\n";
            continue;
        }
        std::cout << "New connection successfull!\n";
        fds[i++].fd = client_socket;
    }
    
*Warning: There are no non-blocking precautions here!*

### Monitoring our sockets
The different events/revents available with poll:

    POLLIN: There is data to read on the file descriptor
    POLLOUT: Data can be written to the file descriptor without blocking
    POLLERR: An error occurred on the file descriptor
    POLLHUP: The file descriptor was closed by the other end
    
The event we're interested in is mostly POLLIN. Let's change our code.

First of, poll reacts when there is an incoming connection on our fd, for this we use the POLLIN event

    struct pollfd fds[SOMAXCONN] = {0};
    fds[0].fd = listen_socket;
    fds[0].events = POLL_IN;

*POLL_IN is the same as POLLIN, they're just defined in different headers*

Let's do the same for our client sockets.

    fds[j].revents = 0; //we need to set revents each time to 0, or else it'll permanently be POLLIN and will trigger the condition every time
    fds[i].events = POLL_IN;
    fds[i++].fd = client_socket;

We also need to change our loop, instead of trying to create a new socket each time we get to the beginning of our loop, we should create one only if fds[0].revents (our listen_socket) = POLLIN. Meaning there is new data on the file descriptor AKA a new client connection.

    struct pollfd fds[SOMAXCONN] = {0};
    fds[0].fd = listen_socket;
    fds[0].events = POLL_IN;
    size_t i = 1;
    while (true)
    {
        int num_events = poll(fds, i, -1); //-1 for no specified delay, meaning it'll continue to wait for an event indefinitely
        for (int j = 0; j < i; j++)//checking each fd for the revent POLLIN
        {
            if (fds[j].revents != POLLIN)
                continue;
            if (fds[j].fd == listen_socket)
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);

                fds[j].revents = 0;// setting back revents to 0
                fds[i].fd = accept(listen_socket, (sockaddr *) &client_addr, &client_addr_len);
                if (fds[i].fd == -1)
                {
                    std::cerr << "Failed to accept incoming connection.\n";
                    fds[i].fd = 0;
                    continue;
                }
                std::cout << "New connection successfull!\n";
                fds[i++].events = POLL_IN; //setting wanted events of new file descriptor as POLLIN
            }
            else
            {
                //What's up next with message treatment
            }
        }
    }

*Still lacks error handling: POLLIN isn't the only value revent can get, it is good to handle the other outcomes such as POLLERR (error on file descriptor)*

## fcntl()
http://manpagesfr.free.fr/man/man2/fcntl.2.html
Manipulates a file descriptor.

    int fcntl(int fd, int cmd);
    
Presented this way, fcntl is quite a mysterious function. We're not going to look into it in details, but one thing fcntl can do is change the flags of the file descriptor and the flags of the file's state.
As mentionned previously, **non-blocking sockets are highly recommended**, this is why we need fcntl().

### How to use fcntl()
It's important to **retrieve our socket's current flags because directly changing them will overwrite.**
Here's how to do so:
    
    int flags = fcntl(listen_socket, F_GETFL);
    
The cmd argument F_GETFL returns the file's status flags.
Then we can use F_SETFL to change them (example: O_RDONLY, O_WRONLY, O_RDWR attributs), fcntl() then takes an extra argument for the function:

    int fcntl(int fd, int cmd, long arg);
We precise as *arg*, our *flags* and (inclusive or -> |) the flag we wish to add: **O_NONBLOCK**

    fcntl(listen_socket, F_SETFL, flags | O_NONBLOCK);
