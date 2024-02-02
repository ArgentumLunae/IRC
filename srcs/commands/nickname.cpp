/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nickname.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/02 17:38:14 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/02 17:49:11 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responsecodes.hpp"
#include <sstream>

void register_nickname(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::stringstream sstream;

	if (tokens.size() <= 1 || tokens[1].empty())
	{
		sstream << ":" << server->get_config().get_host() << " " << ERR_NONICKNAMEGIVEN << " :No nickname given";
		server->msg_to_client(client->get_fd(), sstream.str());
	}
	else if (server->nickname_in_use(tokens[1]))
	{
		sstream << ":" << server->get_config().get_host() << " " << ERR_NICKNAMEINUSE << " " << tokens[1] << " :Nickname already in use";
		server->msg_to_client(client->get_fd(), sstream.str());
	}
	else if (client->set_nickname(tokens[1]) == FAILURE)
	{
		sstream << ":" << server->get_config().get_host() << " " << ERR_ERRONEUSENICKNAME << " " << tokens[1] << " :Erroneus nickname";
		server->msg_to_client(client->get_fd(), sstream.str());
	}
}