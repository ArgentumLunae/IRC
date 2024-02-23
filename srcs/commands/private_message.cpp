/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   private_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/13 13:01:29 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/23 18:20:37 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"

static void	message_to_channel(Client *client, std::string channelName, std::string body, Server *server)
{
	Channel *destination = server->get_channel(channelName);
	std::vector<Client*> recipiants = destination->get_clients();
	std::string message = ":" + client->get_nickname() + " PRIVMSG ";


	if (destination == nullptr)
	{
		send_response_message(client, ERR_NOSUCHNICK, channelName, server);
		return ;
	}
	for (std::vector<Client*>::iterator iter = recipiants.begin(); iter != recipiants.end(); iter++)
	{
		if (*iter == client)
			continue ;
		server->msg_to_client((*iter)->get_fd(), message + channelName + " " + body  + "\r\n");
	}
}

void	private_message(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> destinations;
	std::string message = "";

	if (tokens.size() < 2)
		send_response_message(client, ERR_NORECIPIENT, tokens[0], server);
	else if (tokens.size() < 3)
		send_response_message(client, ERR_NOTEXTTOSEND, tokens[0], server);

	destinations = split(tokens[1], ',');
	for (size_t i = 2; i < (tokens.size() - 1); i++)
		message += tokens[i] + " ";
	message += tokens[tokens.size() - 1];
	std::cout << "MESSAGE " << message << std::endl;

	for (std::vector<std::string>::iterator iter = destinations.begin(); iter != destinations.end(); iter++)
	{
		if ((*iter)[0] == '#' || (*iter)[0] == '&')
			message_to_channel(client, *iter, message, server);
		else
		{
			Client *recipiant = server->get_client(*iter);

			if (recipiant != nullptr)
				server->msg_to_client(recipiant->get_fd(), ":" + client->get_nickname() + " PRIVMSG " + recipiant->get_nickname() + " " + message + "\r\n");
			else
				send_response_message(client, ERR_NOSUCHNICK, *iter, server);
		}
	}
}