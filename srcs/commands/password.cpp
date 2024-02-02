/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   password.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 16:11:56 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/02 17:35:40 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include "server.hpp"
#include "responsecodes.hpp"

#include <sstream>
#include <iostream>

void validate_password(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::stringstream sstream;

	if (tokens.size() <= 1 || tokens[1].empty())
	{
		sstream << ":" << server->get_config().get_host() << " " << ERR_NEEDMOREPARAMS << " " << tokens[0] << " :Not enough parameters";
		server->msg_to_client(client->get_fd(), sstream.str());
	}
	else if (client->get_isRegistred())
	{
		sstream << ":" << server->get_config().get_host() << " " << ERR_ALREADYREGISTRED << " :You may not reregister";
		server->msg_to_client(client->get_fd(), sstream.str());
	}
	else
		client->set_correctPassword(tokens[1]);
}
