/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   username.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/02 17:51:25 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/02 17:53:40 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responsecodes.hpp"
#include <sstream>

void register_user(Client *client, std::vector<std::string> tokens, Server *server)
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
		client->set_username(tokens[1]);
}