#ifndef	REPLIES_HPP

# define REPLIES_HPP

# define RPL_WELCOME(nickname, fullname)						"001 " + nickname + " :Welcome to IRC " + fullname

# define RPL_UMODEIS(client, modes)							"221 " + client + " " + modes

# define RPL_AWAY(client, nick, message)					"301 " + client + " " + nick + " :" + message
# define RPL_CHANNELMODEIS(client, channel, modes)			"324 " + client + " " + channel + " " + modes
# define RPL_CREATIONTIME(client, channel, creationtime)	"329 " + client + " " + channel + " " + creationtime
# define RPL_NOTOPIC(client, channel)						"331 " + client + " " + channel + " :No topic is set"
# define RPL_TOPIC(client, channel, topic)					"332 " + client + " " + channel + " :" + topic
# define RPL_TOPICWHOTIME(client, channel, nick, timestamp)	"333 " + client + " " + channel + " " + nick + " " + timestamp
# define RPL_INVITING(client, nick, channel)				"341 " + client + " " + nick + " " + channel

# define ERR_NOSUCHNICK(client, nick)						"401 " + client + " " + nick + " :No such nick/channel"
# define ERR_NOSUCHSERVER(client, servername)				"402 " + client + " " + servername + " :No such server"
# define ERR_NOSUCHCHANNEL(client, channel)					"403 " + client + " " + channel + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(client, channel)				"404 " + client + " " + channel + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(client, channel)				"405 " + client + " " + channel + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(client, destination)			"407 " + client + " " + destination + " :Duplicate recipients. No message delivered"
# define ERR_NOORIGIN(client)								"409 " + client + " :No origin specified"	
# define ERR_NORECIPIENT(client)							"411 " + client + " :No recipient given (PRIVMSG)"
# define ERR_NOTEXTTOSEND(client)							"412 " + client + " :No text to send"
# define ERR_NONICKNAMEGIVEN(client)						"431 " + client + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(client, nick)					"432 " + client + " " + nick + " :Erroneus nickname"
# define ERR_NICKCOLLISION(client, nick, user, host)		"436 " + client + " " + nick + " :Nickname collision KILL from " + user + "@" + host
# define ERR_USERNOTINCHANNEL(client, channel)				"441 " + client + " " + channel + " :They aren't on that channel" 
# define ERR_NOTONCHANNEL(client, channel)					"442 " + client + " " + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(client, nick, channel)			"443 " + client + " " + nick + " " + channel + " :is already on channel"
# define ERR_NOTREGISTERED(client)							"451 " + client + " :You have not registered"
# define ERR_NEEDMOREPARAMS(client, command)				"461 " + client + " " + command + " :Not enough parameters"
# define ERR_ALREADYREGISTERED(client)						"462 " + client + " :You may not reregister"
# define ERR_PASSWDMISMATCH(client)							"464 " + client + " :Password incorrect"
# define ERR_CHANNELISFULL(client, channel)					"471 " + client + " " + channel + " :Cannot join channel, user limit reached"
# define ERR_INVITEONLYCHAN(client, channel)				"473 " + client + " " + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(client, channel)				"474 " + client + " " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(client, channel)					"475 " + client + " " + channel + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel)							"476 " + channel + " :Bad channel Mask"
# define ERR_CHANOPRIVSNEEDED(client, channel)				"482 " + client + " " + channel + " :You're not channel operator"

# define ERR_UMODEUNKNOWNFLAG(client)						"501 " + client + " :Unknown MODE flag"

// MESSAGE NON-OFFICIEL

# define ERR_NOPRIMORDIAL(client, channel)					client + " " + channel + ": You're not the primordial channel operator"
# define ERR_CANTKICKOPE(client, channel)					client + " " + channel + ": Operators can't be kicked"
# define ERR_AUTH											"You're not authenticated"
# define ERR_NICKNAMEINUSE(nick)							nick + ": Nickname is already in use"

#endif