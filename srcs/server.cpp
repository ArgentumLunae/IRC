#include <iostream>
#include <fcntl.h>
#include "client.hpp"
#include "channel.hpp"
#include "config.hpp"
#include "server.hpp"
#include <errno.h>
#include <cstring>
#include <unistd.h>

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
	serverPoll.events = POLLIN | POLLOUT | POLLHUP;
	_fds.push_back(serverPoll);
	return SUCCESS;;
}

void	Server::runServer()
{
	bool endserver = false;
	int  timeout = 1000;         // Does this need to be part of configuration?

	while (endserver == false)
	{
		int rc;

		std::cout << " waiting on poll()..." << std::endl;
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
		if (_fds[i].revents & POLLIN)
		{
			if (i == 0)
			{
				clientConnect(i);
				continue ;
			}
			else
				incomingData(i);
		}
		if (_fds[i].revents & POLLOUT && i > 0)
				outgoingData(i);
		if (_fds[i].revents & POLLHUP && i > 0)
				clientDisconnect(i);
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
	clientSocket.events = POLLIN | POLLOUT | POLLHUP;
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

void	Server::incomingData(size_t idx)
{
	if (idx)
		return ;
}

void	Server::outgoingData(size_t idx)
{
	if (idx)
		return ;
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

