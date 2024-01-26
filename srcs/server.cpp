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

/* -------- MEMBER FUNCTIONS -------- */

int		Server::initServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

void	Server::runServer()
{
	bool endserver = false;
	int  timeout = 10 * 60 * 1000;         // Does this need to be part of configuration?

	while (endserver == false)
	{
		int rc;

		// std::cout << " waiting on poll()..." << std::endl;
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
		checkRevents();
	}
}

void	Server::closeServer()
{
	std::vector<pollfd>::iterator itend = _fds.end();

	for (std::vector<pollfd>::iterator it = _fds.begin(); it != itend; ++it)
		close(it[0].fd);
	_fds.erase(_fds.begin(), itend);
}

void	Server::checkRevents()
{
	for (size_t i = 0; i < _fds.size(); i++)
	{
		// std::cout << _fds.size() << "\t" << i << std::endl;
		if (_fds[i].revents & POLLIN)
		{
			// std::cout << "POLLIN " << i << std::endl;
			if (i == 0)
			{
				clientConnect(i);
				continue ;
			}
			else if (incomingData(i) == FAILURE)
				continue ;
		}
		if ((_fds[i].revents & POLLOUT) && i > 0)
		{
			// std::cout << "POLLOUT " << i << std::endl;
			outgoingData(_fds[i].fd);
		}
		if ((_fds[i].revents & POLLHUP) && i > 0)
		{
			std::cout << "POLLHUP " << i << std::endl;
			clientDisconnect(i);
		}
	}
}

int	Server::clientConnect(size_t idx)
{
	struct sockaddr clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	pollfd clientSocket;
	
	clientSocket.fd = accept(_fds[idx].fd, &clientAddr, &clientAddrLen);

	if (clientSocket.fd < 0 && errno != EWOULDBLOCK)
	{
		std::cerr << "accept() error: " << strerror(errno) << std::endl;
		return FAILURE;
	}
	std::cout << "New incoming connection - #" << clientSocket.fd << std::endl;
	clientSocket.events = POLLIN | POLLOUT;
	_fds.push_back(clientSocket);
	add_client(clientSocket.fd);
	return SUCCESS;
}

void	Server::clientDisconnect(size_t idx)
{
	std::cout << "Client #" << _fds[idx].fd << " has disconnected." << std::endl;
	remove_client(_fds[idx].fd); //TODO write out this function
	close(_fds[idx].fd);
	_fds.erase(_fds.begin() + idx);
}

int	Server::incomingData(size_t idx)
{
	char buffer[512];
	int bytesRecv = recv(_fds[idx].fd, buffer, sizeof(buffer) - 1, 0);
		
	if (bytesRecv < 0)
		std::cerr << "recv() error: " << strerror(errno) << std::endl;
	else if (bytesRecv > 0)
	{
		// bool messageComplete = false;
		std::vector<std::string> messages;
		
		buffer[bytesRecv] = '\0';
		// if (bytesRecv >= 2 && buffer[bytesRecv - 2] == '\r' && buffer[bytesRecv - 1] == '\n')
		// 	messageComplete = true;
		// std::cout << "incoming data." << std::endl;
		messages = split(buffer, "\r\n");
		for (std::vector<std::string>::iterator iter = messages.begin(); iter != messages.end(); iter++)
		{
			std::string currentMessage = *iter;
			if (currentMessage.length() > 0)
			{
				std::cout << "Received message from client #" << _fds[idx].fd << ": " << currentMessage << std::endl;
				msg_to_client(_fds[idx].fd, currentMessage);
			}
		}
	}
	else
	{
		Client *client = get_client(_fds[idx].fd);

		std::cout << "Other end of socket is closed." << std::endl;
		if (!client->has_incoming_messages())
		{
			clientDisconnect(idx);
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
	client->push_message(msg);
	return SUCCESS;
}

int	Server::outgoingData(int clientfd)
{
	Client* client = get_client(clientfd);

	if (client == nullptr)
	{
		std::cerr << "outgoingData(): Client #" << clientfd << " not found." << std::endl;
		return FAILURE;
	}
	// std::cout << "check if client has incoming messages." << std::endl;
	while (client->has_incoming_messages())
	{
		std::string message = client->pop_message();
		if (message == "CAP LS")
			message = ":127.0.0.1 001 test :CAP * LS :\r\n";
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
	if (this->initServer())
		return FAILURE;
	runServer();
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
	if (nickname.empty())
		return false;
	return false;
}

int 	Server::add_channel(std::string channelName, Client &client)
{
	if (channelName.empty())
	{
		client = client;
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

