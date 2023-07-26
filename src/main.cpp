/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 17:05:43 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/26 18:41:04 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "ircserv.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *tmp;
	int	addrinfo;
	int socketfd;
	int backlog = 11;
	int listenreturn;

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
	listenreturn = listen(socketfd, backlog);
	std::cout << "listen returns: " << listenreturn << std::endl;
	if (listenreturn == -1)
	{
		std::cerr << "listen() error: " << strerror(errno) << std::endl;
		return (EXIT_FAILURE);
	}
	while (true) 
	{
		int clientSocket = accept(socketfd, tmp->ai_addr, &tmp->ai_addrlen);
		if (clientSocket < 0)
		{
			std::cerr << "accept() error" << strerror(errno);
			close(clientSocket);
			continue ;
		}
		fcntl(clientSocket, F_SETFL, O_NONBLOCK);
		int bytesSent = send(clientSocket, "WELCOME TO THE SERVER", 22, MSG_DONTWAIT);
		std::cout << "send() return: " << bytesSent << std::endl;
		if (bytesSent < 0)
		{
			std::cerr << "send() error" << strerror(errno);
			close(clientSocket);
			continue ;
		}
		std::cout << "Client reached end of loop." << std::endl;
		close(clientSocket);
	}
	return (EXIT_SUCCESS);
}
