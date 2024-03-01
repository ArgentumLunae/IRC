/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   part.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/20 14:53:58 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/01 16:50:37 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"
#include "process_message.hpp"

static std::string	construct_part_message(std::vector<std::string> & tokens)
{
	std::string partMessage;

	if (tokens[2][0] != ':')
		return ":" + tokens[2];
	for (size_t i = 2; i < tokens.size(); i++)
		partMessage += " " + tokens[i];
	return partMessage;
}

void	part_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> channelNames;
	std::string	partMessage = "";
	std::string partPrefix;
	std::string msg;

	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NEEDMOREPARAMS, tokens[1], server);
		return ;
	}
	channelNames = split(tokens[1], ',');
	if (tokens.size() > 2)
		partMessage = construct_part_message(tokens);
	partPrefix = ":" + client->get_nickname() + "!" + client->get_username() + "@" + server->get_config().get_host();
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::map<std::string, Channel*>::iterator currentChannel;

		if (server->get_channelList()->empty())
		{
			send_response_message(client, ERR_NOSUCHCHANNEL, channelNames[i], server);
			continue;
		}
		currentChannel = server->get_channelList()->find(channelNames[i]);
		if (currentChannel == server->get_channelList()->end())
		{
			send_response_message(client, ERR_NOSUCHCHANNEL, channelNames[i], server);
			continue ;
		}
		if (!client->is_in_channel(channelNames[i]))
		{
			send_response_message(client, ERR_NOTONCHANNEL, channelNames[i], server);
			continue ;
		}
		msg = partPrefix + " PART " + currentChannel->first;
		if (!partMessage.empty())
			msg += " " + partMessage;
		server->msg_to_client(client->get_fd(), partPrefix + " PART " + currentChannel->first + partMessage);
		currentChannel->second->msg_to_channel(client, partPrefix + " PART " + currentChannel->first + partMessage);
		client->leave_channel(currentChannel->second);
	}
}