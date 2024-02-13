/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   join.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/09 15:37:20 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/13 18:02:26 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>
#include "responseMessage.hpp"
#include "responsecodes.hpp"
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
		currentChannel = server->get_channel(*iter);
		if (currentChannel == nullptr)
			server->add_channel(*iter, *client);
		if (idx < channelKeys.size() && !currentChannel->get_password().empty())
		{
			if (channelKeys[idx] != currentChannel->get_password())
			{
				send_response_message(client, ERR_BADCHANNELKEY, "", server);
				return ;
			}
		}
		std::cout << "|-----DEBUG-----|" << std::endl;
		if (currentChannel->add_client(client))
		{
			std::cout << "|-----DEBUG-----|" << std::endl;
			send_response_message(client, RPL_TOPIC, currentChannel->get_name() + " :" + currentChannel->get_topic(), server);
		}
		idx++;
	}
	 
}