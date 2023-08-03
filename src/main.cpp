/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 17:05:43 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/08/03 15:05:48 by mteerlin      ########   odam.nl         */
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

#define HOST "127.0.0.1"


int main(int argc, char **argv)
{
	int					len, rc, on = 1;
	int					listenSD = -1, newSD = -1;
	int					descReady;
	bool				endServer = false;
	bool				compressArray = false;
	struct sockaddr_in6	addr;
	int					timeout;
	struct pollfd 		fds[200];
	int					nfds = 1;
	int					currentSize = 0;
	int					i, j;

	listenSD = socket(AF_INET6, SOCK_STREAM, 0);
	if (listenSD  < 0)
	{
		std::cerr << "Socket() error: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}

	rc = setsockopt(listenSD, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		std::cerr << "setsockopt() error:" << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}

	rc = fcntl(listenSD, O_NONBLOCK, (char *)on);
	if (rc < 0)
	{
		std::cerr << "fcntl() error:" << strerror(errno) << std::endl;
		close(listenSD);
		return (EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memccpy()


	return (EXIT_SUCCESS);
}
