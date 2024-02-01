/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   capabilities.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/30 16:54:13 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/01 16:29:28 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <sstream>
#include <sys/socket.h>
#include "server.hpp"
#include "client.hpp"
#include "responsecodes.hpp"
#include <cstring>

#include <iostream>

void capabilities(int clientfd, std::vector<std::string> tokens, Server *server)
{
	if (tokens[1] == "LS")
		server->msg_to_client(clientfd, "CAP * LS :\r\n");
	else if (tokens[1] == "END")
		server->msg_to_client(clientfd, ":127.0.0.1 001 server :Welcome to the server\r\n");
	else
	{
		std::stringstream errorStream;
		std::string error;

		errorStream << ":" << server->get_config().get_host() << " " << ERR_INVALIDCAPCMD << " * " << tokens[1] << " :Invalid CAP command." << std::endl;
		error = errorStream.str();
		server->msg_to_client(clientfd, error);
	}
}