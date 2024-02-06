/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nickname.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 14:33:26 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/06 17:30:29 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"

static bool is_allowed_character(char c)
{
	if ((c >= 'a' && c <= 'z') || \
			(c >= 'A' && c <= 'Z') || \
			(c >= '0' && c <= '9') || \
			c == '!' || c == '_' || c == '-')
		return true;
	return false;
}

void	register_nickname(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NONICKNAMEGIVEN, "NICK ", server);
		return ;
	}
	if (tokens[1] == client->get_nickname())
		return ;
	for (int i = 0; tokens[1][i]; i++)
	{
		if (!is_allowed_character(tokens[1][i]))
		{
			send_response_message(client, ERR_ERRONEUSENICKNAME, tokens[1], server);
			server->msg_to_client(client->get_fd(), "Nickname may only include a-z, A-Z, 0-9, !, -, and _.\r\n");
			return ;
		}
	}
	if (tokens[1].length() > 9)
		server->msg_to_client(client->get_fd(), "Nickname too long, maximum of 9 characters.");
	else if (server->nickname_in_use(tokens[1]))
		send_response_message(client, ERR_NICKNAMEINUSE, tokens[1], server);
}