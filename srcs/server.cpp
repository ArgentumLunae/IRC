#include <iostream>
#include <fcntl.h>
#include "client.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "server.hpp"

//CONSTRUCTOR/DECONSTRUCTOR
Server::Server(std::string setpass, int setport)
{
    //Validation for these things? no idea if it's needed or not
	_pass = setpass;
	_port = setport;
}

Server::~Server()
{
	return ;
}

// GETTERS
std::string	Server::get_name() const
{
	return (_name);
}

Config	Server::get_config() const
{
	return (_config);
}

std::string	Server::get_pass() const
{
	return (_pass);
}

int	Server::get_port() const
{
	return (_port);
}

std::map<int, Client>* Server::get_clientList()
{
	return (&_clientList);
}

//in case the client isn't in the list at all, chuck a nullptr back? probably a better way to do this
//I use this method for all the others, should ask Michiel at some point if he has a better idea.
Client*	Server::get_client(int fd)
{
	std::map<int, Client>::iterator client = _clientList.find(fd);
	if (client == _clientList.end())
		return (nullptr);
	return (&client->second);
}

Client* Server::get_client(std::string nickname)
{
	for (std::map<int, Client>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
	{
		if (it->second.get_nickname() == nickname)
			return (&it->second);
	}
	return (nullptr);
}

std::map<std::string, Channel>* Server::get_channelList()
{
	return (&_channelList);
}

Channel*	Server::get_channel(std::string channelName)
{
	std::map<std::string, Channel>::iterator channel = _channelList.find(channelName);
	if (channel == _channelList.end())
		return (nullptr);
	return (&channel->second);
}
//SETTERS

//OTHERS
int 	add_client(int fd);
int	    remove_client(int fd);
bool	nickname_in_use(std::string nickname);
int 	add_channel(std::string channelName, Client &client);
int	    remove_channel(std::string channelName);
int		start_server();
