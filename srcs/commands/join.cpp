/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   join.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/09 15:37:20 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/29 18:26:54 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "process_message.hpp"
#include "utils.h"

// TODO: finish working on this

void	join_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> channelNames;
	std::vector<std::string> channelKeys;
	Channel	*currentChannel;
	size_t	idx = 0;

	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NEEDMOREPARAMS, "JOIN ", server);
		return ;
	}
	channelNames = split(tokens[1], ',');
	if (tokens.size() > 2)
		channelKeys = split(tokens[2], ',');
	for (std::vector<std::string>::iterator iter = channelNames.begin(); iter != channelNames.end(); iter++)
	{
		if (!(server->add_channel(*iter, *client) == SUCCESS))
		{
			std::cout << "Failed to find or create channel." << std::endl;
			return ;
		}
		else
			currentChannel = server->get_channel(*iter);
		if (idx < channelKeys.size() && !currentChannel->get_password().empty()
			&& channelKeys.at(idx) != currentChannel->get_password())
		{
				send_response_message(client, ERR_BADCHANNELKEY, "", server);
				return ;
		}
		if (!currentChannel->add_client(client))
		{
			if (currentChannel->get_topic().empty())
				topic_command(client, std::vector<std::string>{"TOPIC", *iter}, server);
			else
				topic_command(client, std::vector<std::string>{"TOPIC", *iter, currentChannel->get_topic()}, server);
		}
		idx++;
	}
	list_names(client, tokens, server);
}