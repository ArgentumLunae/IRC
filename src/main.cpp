/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 17:05:43 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/28 17:42:24 by mteerlin      ########   odam.nl         */
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

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *tmp;
	int	addrinfo;
	int socketfd;
	int backlog = 11;

	(void)argv;
	if (argc != 3)
	{
		std::cout << "Usecase: ./ircserv <ServerPort> <ServerPassword>";
		return (EXIT_FAILURE);
	}
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = getprotobyname("TCP")->p_proto;
	std::cout << "getprotobyname(\"TCP\") -> " << hints.ai_protocol << std::endl;
	addrinfo = getaddrinfo("127.0.0.1", argv[1], &hints, &res);
	for (tmp = res; tmp != NULL; tmp = tmp->ai_next)
	{
		socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketfd < 0)
		{
			std::cout << "socket() error: " << strerror(errno) << std::endl;
			continue ;
		}
		if (bind(socketfd, tmp->ai_addr, tmp->ai_addrlen) < 0)
		{
			std::cout << "socket failed to bound to name: " << socketfd << std::endl;	
			close(socketfd);
			continue ;
		}
		std::cout << "socket bound to name: " << socketfd << std::endl;
		break ;
	}
	if (listen(socketfd, backlog) == -1)
	{
		std::cerr << "listen() error: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}
	int idx = 1;
	pollfd	clientSocket;
	while (true) 
	{
		// std::cout << "starting loop number: " << idx << std::endl;
		fcntl(socketfd, F_SETFL, O_NONBLOCK);
		if (!clientSocket.fd)
			clientSocket.fd = accept(socketfd, tmp->ai_addr, &tmp->ai_addrlen);
		fcntl(clientSocket.fd, F_SETFL, O_NONBLOCK);
		// std::cout << "accept is blocking" << std::endl;
		// if (clientSocket.fd < 0)
		// {
		// 	std::cerr << "accept() error " << strerror(errno) << " " << clientSocket.fd << std::endl;
		// 	close(clientSocket.fd);
		// 	continue ;
		// }
		int bytesSent = send(clientSocket.fd, ":127.0.0.1 001 test :WELCOME TO THE SERVER\n", 43, MSG_DONTWAIT);
		send(clientSocket.fd, ":127.0.0.1 372 test :This is your message of the day\n", 53, MSG_DONTWAIT);
		send(clientSocket.fd, ":127.0.0.1 372 test :TWF IS GOING ON\n", 37, MSG_DONTWAIT);
		std::cout << "send() return: " << bytesSent << std::endl;
		if (bytesSent < 0)
		{
			std::cerr << "send() error" << strerror(errno) << std::endl;
			close(clientSocket.fd);
			continue ;
		}
		char buffer[1024];
		int	bytesread = 0;
		if (poll(&clientSocket, 1, 1000) && (bytesread = recv(clientSocket.fd, buffer, sizeof(buffer), 0)) > 0)
		{
			std::string receivedMessage(buffer, bytesread);
			std::cout << "Message from client: " << receivedMessage << std::endl;
		}
		if (bytesread == 0)
		{
			std::cout << "Client Disconnected." << std::endl;
		} else {
			std::cerr << "Error in receiving data from client." << std::endl;
		}

		std::cout << "Client reached end of loop." << std::endl;
		// close(clientSocket.fd);
		idx++;
	}
	return (EXIT_SUCCESS);
}
