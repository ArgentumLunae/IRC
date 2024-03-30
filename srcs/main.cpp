/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/10 11:48:33 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/30 17:12:50 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <cstring>

#include <string>
#include <iostream>
#include <array>	

#include "../includes/server.hpp"


// expected input: ./ircserv <port> <password>
	// port: port to listen for incomming connections
	// password: password needed to connect to server
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usecase: ./ircserc <server_port> <server_password>" << std::endl;
		return EXIT_SUCCESS;
	}

	std::string portStr(argv[1]);
	std::string password(argv[2]);
	int			port;

	try {
		port = std::stoi(portStr);
	}
	catch (...) {
		std::cout << "Usecase: ./ircserc <server_port> <server_password>" << std::endl;
		std::cout << "Usecase: <server_port> must be numeric" << std::endl;
		return EXIT_SUCCESS;
	}
	Server server(password,  port);	

	server.start_server();
	std::cout << "Program finished" << std::endl;
	return EXIT_SUCCESS;
}