/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   private_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/13 13:01:29 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/29 16:16:12 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"

static void	message_to_channel(Client *client, std::string channelName, std::string body, Server *server)
{
	Channel *destination = server->get_channel(channelName);
	std::vector<Client*> recipiants;
	std::string prefix = ":" + client->get_nickname() + " PRIVMSG ";

	if (destination == nullptr)
	{
		send_response_message(client, ERR_NOSUCHNICK, ":" + channelName, server);
		return ;
	}
	recipiants = destination->get_clients();
	if (!client->is_in_channel(channelName) && destination->client_rejoin(client) != SUCCESS)
	{
		send_response_message(client, ERR_CANNOTSENDTOCHAN, ":" + channelName, server);
		return ;
	}
	destination->msg_to_channel(client, prefix + channelName + " " + body);
}

void	private_message(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> destinations;
	std::string message = "";

	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NORECIPIENT, tokens[0], server);
		return ;
	}
	else if (tokens.size() < 3)
	{
		send_response_message(client, ERR_NOTEXTTOSEND, tokens[0], server);
		return ;
	}

	destinations = split(tokens[1], ',');
	if (tokens[2][0] != ':')
		message = tokens[2];
	else
	{
		for (size_t i = 2; i < (tokens.size() - 1); i++)
			message += tokens[i] + " ";
		message += tokens[tokens.size() - 1];
	}

	for (std::vector<std::string>::iterator iter = destinations.begin(); iter != destinations.end(); iter++)
	{
		if ((*iter)[0] == '#' || (*iter)[0] == '&')
			message_to_channel(client, *iter, message, server);
		else
		{
			Client *recipiant = server->get_client(*iter);

			if (recipiant != nullptr)
				server->msg_to_client(recipiant->get_fd(), ":" + client->get_nickname() + " PRIVMSG " + recipiant->get_nickname() + " " + message);
			else
				send_response_message(client, ERR_NOSUCHNICK, ":" + *iter, server);
		}
	}
}