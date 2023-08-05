/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 17:05:43 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/08/05 15:06:50 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "ircserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <iostream>

/*taking inventory of shit used
	- addrinfo for hints, result, and a temporary one to run through different options (may not all be required)
	- the socketfd of the server
	- backlog, I assume how many messages can wait in the cue at a time (but memory if fucked)

	- a socketfd for the client

	- The host name. hardcoded '127.0.0.1' in this example.
	- the port and password, as given in argv.
*/

#define MOTDPRELEN 22
#include <array>

int	sendMOTD(int &recipient)
{
	std::string	serverPrefix = ":127.0.0.1 001 test :";
	std::string	endLine = "\r\n";
	std::string MOTD[5] = {
		serverPrefix + "Welcome to our IRC server" + endLine,
		serverPrefix + "-------------------------" + endLine,
		serverPrefix + "|         IRC           |" + endLine,
		serverPrefix + "-------------------------" + endLine,
		serverPrefix + "Do whatever" + endLine
	};

	for (int i = 0; i < 5; i++)
	{
		if (send(recipient, (MOTD[i]).c_str(), (MOTD[i]).length(), 0) < 0)
		{
			std::cerr << "send() error: " << strerror(errno) << std::endl;
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int					len, rc, on = 1;
	int					listenSD = -1, newSD = -1;		// listenSD == server socket descriptor, newSD will be incomming client socket descriptors
	// int					descReady;
	bool				endServer = false;				// flag to indicate server close is required.
	bool				compressArray = false;			// flag indicating an element must be removed from the fds[] array
	bool				closeCon = false;				// flag indicating a connection is to be closed
	char 				buffer[80];						// buffer for incomming text.
	struct sockaddr_in6	addr;
	int					timeout;						// amount of time poll will wait for a response.
	struct pollfd 		fds[200];						// list of all the open sockets connected to the program, starting with [0] being the server socket
	int					nfds = 1;						// number of currently connected sockets, including server socket.
	int					currentSize = 0;
	int					i, j;
	std::string			serverPrefix = ":127.0.0.1 001 test :";
	std::string			endLine = "\r\n";
	
	if (argc != 3)
	{
		std::cout << "Usecase: ./ircserv <server_port> <server_password>" << std::endl;
		return (EXIT_SUCCESS);
	}

	std::string			port = argv[1];

	listenSD = socket(AF_INET6, SOCK_STREAM, 0);
	if (listenSD  < 0)
	{
		std::cerr << "Socket() error: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}

	rc = setsockopt(listenSD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cerr << "setsockopt() error: " << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}

	rc = fcntl(listenSD, O_NONBLOCK, (char *)&on);
	if (rc < 0)
	{
		std::cerr << "fcntl() error: " << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}

	std::memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	std::memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(std::stoi(port));
	rc = bind(listenSD, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		std::cerr << "bind() error: " << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}

	rc = listen(listenSD, 32);
	if (rc < 0)
	{
		std::cerr << "listen() error: " << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}
	
	std::memset(fds, 0, sizeof(fds));

	fds[0].fd = listenSD;
	fds[0].events = POLLIN;

	timeout = (3 * 60 * 1000);

	do
	{
		std::cout << "	waiting on pol()..." << std::endl;
		rc = poll(fds, nfds, timeout);
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

		currentSize = nfds;
		for (i = 0; i < nfds; i++)
		{
			if (fds[i].revents == 0)
				continue ;
			if (fds[i].revents != POLLIN)
			{
				std::cerr << "Error! revents = " << fds[i].revents << std::endl;
				endServer = true;
				break ;
			}
			if (fds[i].fd == listenSD)
			{
				std::cout << "	Listening socket is readable" << std::endl;
				do
				{
					newSD = accept(listenSD, NULL, NULL);
					if (newSD < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "accept() error: " << strerror(errno) << std::endl;
							endServer = true;
						}
						break ;
					}
				
					std::cout << "	new incoming connection - " << newSD << std::endl;
					fds[nfds].fd = newSD;
					fds[nfds].events = POLLIN;
					nfds++;
					if (sendMOTD(newSD))
						closeCon = true;
				} while (newSD != -1);
			}
			else
			{
				std::cout << "	Descriptor " << fds[i].fd << " is readable" << std::endl;
				closeCon = false;

				do
				{
					rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cerr << "recv() error " << strerror(errno) << std::endl;
							closeCon = true;
						}
						break ;
					}
					if (rc == 0)
					{
						std::cout << "Connection closed" << std::endl;
						closeCon = true;
						break ;
					}
					len = rc;
					std::cout << "	" << len << " bytes received" << std::endl;
					if (len < 80)
						buffer[len] = '\n';
					rc = send (fds[i].fd, serverPrefix.c_str(), len, 0);
					rc = send (fds[i].fd, buffer, len, 0);
					if (rc < 0)
					{
						std::cerr << "send() error: " << strerror(errno) << std::endl;
						closeCon = true;
						break ;
					}
					std::cout << "	" << len << " bytes sent" << std::endl;
				} while (true);
				
				rc = send (fds[i].fd, buffer, len, 0);
					if (rc < 0)
					{
						std::cerr << "send() error: " << strerror(errno) << std::endl;
						closeCon = true;
						break ;
					}
					std::cout << "	" << len << " bytes sent" << std::endl;

				if (closeCon)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					compressArray = true;
				}
			}
		}

		if (compressArray)
		{
			compressArray = false;
			for (i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for(j = i; j < nfds; j++)
					{
						fds[j].fd = fds[j + 1].fd;
					}
					i--;
					nfds--;
				}
			}
		}
	} while (endServer == false);
	
	for (i = 0; i < nfds; i++)
	{
		if (fds[i].fd >= 0)
			close(fds[i].fd);
	}

	return (EXIT_SUCCESS);
}
