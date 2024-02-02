/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   capabilities.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/30 16:54:13 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/02 18:06:38 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <sstream>
#include <sys/socket.h>
#include "server.hpp"
#include "responsecodes.hpp"
#include <cstring>

#include <iostream>

void capabilities(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens[1] == "LS")
	{
		client->set_capabilityNegotiation(true);
		server->msg_to_client(client->get_fd(), "CAP * LS :\r\n");
	}
	else if (tokens[1] == "END")
	{
		client->set_capabilityNegotiation(false);
		server->msg_to_client(client->get_fd(), ":127.0.0.1 001 server :Welcome to the server\r\n");
	}
	else
	{
		std::stringstream errorStream;
		std::string error;

		errorStream << ":" << server->get_config().get_host() << " " << ERR_INVALIDCAPCMD << " * " << tokens[1] << " :Invalid CAP command." << std::endl;
		error = errorStream.str();
		server->msg_to_client(client->get_fd(), error);
	}
}