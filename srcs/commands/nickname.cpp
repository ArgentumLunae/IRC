/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nickname.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 14:33:26 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/14 17:53:30 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"

static bool validate_nick_syntax(std::string const nick)
{
	for (int i = 0; nick[i]; i++)
	{
		if ((nick[i] >= 'a' && nick[i] <= 'z') || \
			(nick[i] >= 'A' && nick[i] <= 'Z') || \
			(nick[i] >= '0' && nick[i] <= '9') || \
			nick[i] == '!' || nick[i] == '_' || nick[i] == '-')
		return true;
	}
	return false;
}

void	register_nickname(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2 || tokens[1].empty())
	{
		send_response_message(client, ERR_NONICKNAMEGIVEN, "NICK ", server);
		return ;
	}
	if (tokens[1] == client->get_nickname())
		return ;
	if (tokens[1].length() > 9)
		server->msg_to_client(client->get_fd(), "Nickname too long, maximum of 9 characters.");
	else if (!validate_nick_syntax(tokens[1]))
	{
		send_response_message(client, ERR_ERRONEUSENICKNAME, tokens[1], server);
		server->msg_to_client(client->get_fd(), "Nickname may only include a-z, A-Z, 0-9, !, -, and _.\r\n");
	}
	else if (server->nickname_in_use(tokens[1]))
		send_response_message(client, ERR_NICKNAMEINUSE, tokens[1], server);
	else
	{
		if (client->is_registered())
			server->msg_to_client(client->get_fd(), ":" + client->get_nickname() + " NICK :" + tokens[1] + "\r\n");
		client->set_nickname(tokens[1]);
	}
}