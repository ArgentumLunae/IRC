/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 17:20:56 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/08 17:05:33 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "client.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "server.hpp"
#include "utils.h"
#include "process_message.hpp"

/* -------- MEMBER FUNCTIONS -------- */

int		Server::init_server()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	_config.config_from_file("/home/mteerlin/Documents/IRC/gh_irc/configs/serverconfig.conf");
	_config.print_config();
	if (_serverSocket < 0)
	{
		std::cerr << "socket() error: " << strerror(errno) << std::endl;
		return FAILURE;
	}
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "fctnl() error: " << strerror(errno) << std::endl;
		close(_serverSocket);
		return FAILURE;
	}
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) != 0)
	{
		std::cerr << "bind() error: " << strerror(errno) << std::endl;
		close(_serverSocket);
		return FAILURE;
	}
	if (listen(_serverSocket, 10) != 0)
	{
		std::cerr << "listen() error: " << strerror(errno) << std::endl;
		close(_serverSocket);
		return FAILURE;
	}
	std::cout << "Server listening on port " << _port << std::endl;
	
	pollfd serverPoll;
	serverPoll.fd = _serverSocket;
	serverPoll.events = POLLIN | POLLOUT;
	_fds.push_back(serverPoll);
	return SUCCESS;;
}

void	Server::run_server()
{
	bool endserver = false;
	int  timeout = 10 * 60 * 1000;         // Does this need to be part of configuration?

	while (endserver == false)
	{
		int rc;

		rc = poll(_fds.data(), _fds.size(), timeout);
		if (rc < 0)
		{
			std::cerr << "poll() error: " << strerror(errno) << std::endl;
			break ;
		}
		if (rc == 0)
		{
			std::cerr << "poll() timeout: " << strerror(errno) << std::endl;
			break ;
		}
		check_revents();
	}
}

void	Server::closeServer()
{
	std::vector<pollfd>::iterator itend = _fds.end();

	for (std::vector<pollfd>::iterator it = _fds.begin(); it != itend; ++it)
		close(it[0].fd);
	_fds.erase(_fds.begin(), itend);
}

void	Server::check_revents()
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		if (_fds[i].revents & POLLIN)
		{
			if (i == 0)
			{
				client_connect();
				continue ;
			}
			else if (incoming_data(i) == FAILURE)
				continue ;
		}
		if ((_fds[i].revents & POLLOUT) && i > 0)
			outgoing_data(_fds[i].fd);
		if ((_fds[i].revents & POLLHUP) && i > 0)
			client_disconnect(_fds[i].fd);
	}
}

int	Server::client_connect()
{
	struct sockaddr clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	pollfd clientSocket;
	
	clientSocket.fd = accept(_serverSocket, &clientAddr, &clientAddrLen);

	if (clientSocket.fd < 0 && errno != EWOULDBLOCK)
	{
		std::cerr << "accept() error: " << strerror(errno) << std::endl;
		return FAILURE;
	}
	std::cout << "New incoming connection - #" << clientSocket.fd << std::endl;
	clientSocket.events = POLLIN | POLLOUT;
	add_client(clientSocket.fd);
	_fds.push_back(clientSocket);
	return SUCCESS;
}

void	Server::finish_client_registration(Client *client)
{
	if (client->get_correctPassword() == false || client->get_nickname().empty() || client->get_username().empty())
	{
		if (!client->get_capabilityNegotiation())
			client_disconnect(client->get_fd());
		return ;
	}
	client->set_registered(true);
	msg_to_client(client->get_fd(), ":" + _config.get_host() + " 001 " + _config.get_serverName() + " :Welcome to the server\r\n");
}


void	Server::client_disconnect(int clientfd)
{
	std::cout << "Client #" << clientfd << " has disconnected." << std::endl;
	remove_client(clientfd); //TODO write out this function
	close(clientfd);
	for (std::vector<pollfd>::iterator iter = _fds.begin(); iter != (_fds.end() - 1); iter++)
	{
		if ((*iter).fd == clientfd)
			_fds.erase(iter);
	}
}

int	Server::incoming_data(size_t idx)
{
	char buffer[512];
	Client *client = get_client(_fds[idx].fd);
	int clientfd = client->get_fd();
	int bytesRecv = recv(clientfd, buffer, sizeof(buffer) - 1, 0);
		
	if (bytesRecv < 0)
		std::cerr << "recv() error: " << strerror(errno) << std::endl;
	else if (bytesRecv > 0)
	{
		bool messageComplete = false;
		std::vector<std::string> messages;
		
		buffer[bytesRecv] = '\0';
		if (bytesRecv >= 2 && buffer[bytesRecv - 2] == '\r' && buffer[bytesRecv - 1] == '\n')
		{
			messageComplete = true;
			std::cout << "messageComplete == true" << std::endl;
		}
		std::cout << "incoming data." << std::endl;
		messages = split(buffer, "\r\n");
		for (std::vector<std::string>::iterator iter = messages.begin(); iter != (messages.end() - 1); iter++)
		{
			std::string currentMessage = *iter;
			if (currentMessage.length() > 0)
			{
				std::cout << "Received message from client #" << clientfd << ": " << currentMessage << std::endl;
				if (iter == messages.begin())
				{
					currentMessage = client->get_messageBuffer() + currentMessage;
					client->clear_message_buffer();
				}
				process_message(get_client(clientfd), currentMessage, client->get_server());
				client->clear_message_buffer();
			}
		}
		if (messageComplete == true)
		{
			std::string finalMessage = client->get_messageBuffer() + messages.back();

			if(finalMessage.length() > 0)
			{
				std::cout << "Received final message from client #" << _fds[idx].fd << ": " << finalMessage << std::endl;
				process_message(get_client(clientfd), finalMessage, client->get_server());
			}
		}
		else 
		{
			std::string finalMessage = messages.back();

			if(finalMessage.length() > 0)
			{
				std::cout << "Received partial message from client #" << _fds[idx].fd << ": " << finalMessage << std::endl;
				client->add_to_message_buffer(finalMessage);
			}
		}
	}
	else
	{
		std::cout << "Other end of socket is closed." << std::endl;
		if (!client->has_incoming_messages())
		{
			client_disconnect(_fds[idx].fd);
			return FAILURE;
		}
	}
	std::cout << " " << bytesRecv << " bytes received." << std::endl;
	return SUCCESS;
}

int	Server::msg_to_client(int clientfd, std::string msg)
{
	Client* client = get_client(clientfd);

	if (client == nullptr)
	{
		std::cerr << "msg_to_client(): Client #" << clientfd << " not found." << std::endl;
		return FAILURE;
	}
	std::cout << "Message sent to Client: \"" << msg << "\"" << std::endl;
	client->push_message(msg);
	return SUCCESS;
}

int	Server::outgoing_data(int clientfd)
{
	Client *client = get_client(clientfd);

	if (client == nullptr)
	{
		std::cerr << "outgoing_data(): Client #" << clientfd << " not found." << std::endl;
		return FAILURE;
	}
	while (client->has_incoming_messages())
	{
		std::string message = client->pop_message();
		if (send(clientfd, message.c_str(), message.length(), 0) < 0)
		{
			std::cerr << "send() error: " << strerror(errno) << std::endl;
			return FAILURE;
		}
		else
			std::cout << "Client #" << client->get_fd() << " receives: " << message << std::endl;
	}
	return SUCCESS;
}

int		Server::start_server()
{
	if (this->init_server())
		return FAILURE;
	run_server();
	closeServer();
	return SUCCESS;
}

int 	Server::add_client(int fd)
{
	if (get_client(fd) != nullptr)
		return (-1);	// look into using specific defines for this.
	Client client(fd, this);
	_clientList.insert(std::make_pair(fd, client));
	std::cout << "Client added to server client list" << std::endl;
	return SUCCESS;
}

int	    Server::remove_client(int fd)
{
	// THIS NEEDS TO:
	//disconnect client from all connected channels
	//associated client lists
	//double check if required to be removed from operator lists
	if (get_client(fd) == nullptr)
		return (-1);
	_clientList.erase(_clientList.find(fd));
	return SUCCESS;	
}

bool	Server::nickname_in_use(std::string nickname)
{
	for (std::map<int, Client>::iterator iter = _clientList.begin(); iter != _clientList.end(); iter++)
	{
		if (nickname == (*iter).second.get_nickname())
			return true;
	}
	return false;
}

int 	Server::add_channel(std::string channelName, Client &client)
{
	if (channelName.empty())
	{
		if (client.get_fd())
			return -1;
	}
	return SUCCESS;
}

int	    Server::remove_channel(std::string channelName)
{
	if (channelName.empty())
		return -1;
	return SUCCESS;
}

void	Server::increment_nfds() {
	_nfds++;
}

void	Server::decrement_nfds() {
	_nfds--;
}

/* -------- CONSTRUCTORS & DESTRUCTOR -------- */
Server::Server(std::string setpass, int setport)
{
    //TODO: password and port validation
	_pass = setpass;
	_port = setport;
}

Server::~Server()
{
	return ;
}

/* -------- GETTERS -------- */
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
/* -------- SETTERS -------- */

