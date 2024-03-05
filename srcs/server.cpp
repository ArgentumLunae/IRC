/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 17:20:56 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/05 16:44:10 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "server.hpp"
#include "utils.h"
#include "process_message.hpp"
#include "responseMessage.hpp"

/* -------- MEMBER FUNCTIONS -------- */

int		Server::init_server()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	_config.config_from_file("configs/serverconfig.conf");
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
	int  timeout = 2 * 60 * 1000;         // Does this need to be part of configuration?

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
		if ((_fds[i].revents & POLLRDHUP) && i > 0)
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
	clientSocket.events = POLLIN | POLLOUT | POLLRDHUP;
	add_client(clientSocket.fd);
	_fds.push_back(clientSocket);
	return SUCCESS;
}

int	Server::finish_client_registration(Client *client)
{
	if (client->get_capabilityNegotiation())
		return SUCCESS;
	if (client->get_correctPassword() == false || client->get_nickname().empty() || client->get_username().empty())
		return FAILURE;
	client->set_registered(true);
	msg_to_client(client->get_fd(), ":" + _config.get_host() + " 001 " + client->get_nickname() + " :Welcome to the server");
	return SUCCESS;
}

void	Server::client_disconnect(int clientfd)
{
	remove_client(clientfd);
	close(clientfd);
	for (std::vector<pollfd>::iterator iter = _fds.begin(); iter != _fds.end(); ++iter)
	{
		if (iter->fd == clientfd)
		{
			_fds.erase(iter);
			std::cout << "Client #" << clientfd << " has disconnected." << std::endl;
			return ;
		}
	}
	std::cout << "Client #" << clientfd << " not found." << std::endl;
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
			messageComplete = true;
		messages = split(buffer, '\n');
		for (std::vector<std::string>::iterator iter = messages.begin(); iter != (messages.end() - 1); iter++)
		{
			std::string currentMessage = *iter;
			if (currentMessage.length() > 0 && !(currentMessage[0] == '\r'))
			{
				if (currentMessage[currentMessage.length() - 1] == '\r')
					currentMessage = currentMessage.substr(0, currentMessage.length() - 1);
				std::cout << "Received message from client #" << clientfd << ": [" << currentMessage << "]" << std::endl;
				if (iter == messages.begin())
				{
					currentMessage = client->get_messageBuffer() + currentMessage;
					client->clear_message_buffer();
				}
				if (process_message(get_client(clientfd), currentMessage, client->get_server()) == FAILURE)
					continue ;
				client->clear_message_buffer();
			}
		}
		if (messageComplete == true)
		{
			std::string finalMessage = client->get_messageBuffer() + messages.back();

			if(finalMessage.length() > 0 && finalMessage[0] != '\r')
			{
				finalMessage = finalMessage.substr(0, finalMessage.length() - 1);
				std::cout << "Received final message from client #" << _fds[idx].fd << ": [ " << finalMessage << " ]" << std::endl;
				process_message(get_client(clientfd), finalMessage, client->get_server());
				client->clear_message_buffer();
			}
		}
		else 
		{
			std::string finalMessage = messages.back();

			if(finalMessage.length() > 0)
			{
				std::cout << "Received partial message from client #" << _fds[idx].fd << ": [ " << finalMessage << " ]" << std::endl;
				client->add_to_message_buffer(finalMessage);
			}
		}
	}
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

void	Server::broadcast(std::string msg)
{
	for (std::map<int, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); iter++)
		msg_to_client(iter->first, msg);
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
		std::string message = client->pop_message() + "\r\n";
		if (send(clientfd, message.c_str(), message.length(), MSG_NOSIGNAL) < 0)
		{
			std::cerr << "send() error: " << strerror(errno) << std::endl;
			return FAILURE;
		}
		else
			std::cout << "Client #" << client->get_fd() << " receives: \"" << message.substr(0, message.length() - 2) << "\"" << std::endl;
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
	std::cout << "|" << fd << "|" << std::endl;
	if (get_client(fd) != nullptr)
		return (-1);	// look into using specific defines for this.
	Client *client = new Client(fd, this);
	_clientList.insert(std::make_pair(fd, client));
	return SUCCESS;
}

int	    Server::remove_client(int fd)
{
	// THIS NEEDS TO:
	Client *client = get_client(fd);
	if (client == nullptr)
		return (-1);
	client->leave_all_channels();
	delete client;
	_clientList.erase(fd);
	return SUCCESS;	
}

void	Server::remove_all_clients()
{
	for (std::map<int, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); iter = _clientList.begin())
	{
		remove_client(iter->first);
		if (_clientList.empty())
			break ;
	}
}

bool	Server::nickname_in_use(std::string nickname)
{
	for (std::map<int, Client*>::iterator iter = _clientList.begin(); iter != _clientList.end(); iter++)
	{
		if (nickname == (*iter).second->get_nickname())
			return true;
	}
	return false;
}

int 	Server::add_channel(std::string channelName, Client &client)
{
	if (channelName.empty())
		return FAILURE;
	if (get_channel(channelName) != nullptr)
		return SUCCESS;
	Channel *newChannel = new Channel(channelName, &client, this);
	client.add_channel(newChannel);
	_channelList.insert(std::make_pair(channelName, newChannel));
	return SUCCESS;
}

int	    Server::remove_channel(std::string channelName)
{
	std::map<std::string, Channel*>::iterator channelIter = _channelList.find(channelName);

	if (channelIter == _channelList.end())
		return FAILURE;
	delete _channelList.at(channelName);
	_channelList.erase(channelName);
	return SUCCESS;
}

void		Server::remove_all_channels()
{
	for (std::map<std::string, Channel*>::iterator iter = _channelList.begin(); iter != _channelList.end(); iter = _channelList.begin())
	{
		delete iter->second;
		_channelList.erase(iter->first);
		if (_channelList.empty())
			break ;
	}
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
	remove_all_clients();
	remove_all_channels();
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

std::map<int, Client*> Server::get_clientList()
{
	return (_clientList);
}


Client*	Server::get_client(int fd)
{
	std::map<int, Client*>::iterator client = _clientList.find(fd);
	if (client == _clientList.end())
		return (nullptr);
	return (client->second);
}

Client* Server::get_client(std::string nickname)
{
	for (std::map<int, Client*>::iterator it = _clientList.begin(); it != _clientList.end(); it++)
	{
		if (it->second->get_nickname() == nickname)
			return (it->second);
	}
	return (nullptr);
}

std::map<std::string, Channel*>* Server::get_channelList()
{
	return (&_channelList);
}

Channel*	Server::get_channel(std::string channelName)
{
	std::map<std::string, Channel*>::iterator channel = _channelList.find(channelName);
	if (channel == _channelList.end())
		return (nullptr);
	return (channel->second);
}

/* -------- SETTERS -------- */

