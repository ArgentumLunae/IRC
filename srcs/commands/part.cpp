/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   part.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/20 14:53:58 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/23 12:33:47 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"

void	part_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> channelNames;

	std::cout << "part_command()" << std::endl;
	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NEEDMOREPARAMS, tokens[1], server);
		return ;
	}
	channelNames = split(tokens[1], ',');
	std::cout << "channelNames.size() = " << channelNames.size() << std::endl;
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		std::map<std::string, Channel*>::iterator currentChannel;

		if (server->get_channelList()->empty())
		{
			std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
			send_response_message(client, ERR_NOSUCHCHANNEL, channelNames[i], server);
			continue;
		}
		currentChannel = server->get_channelList()->find(channelNames[i]);
		std::cout << "currentChannel = " << currentChannel->first  << std::endl;
		if (currentChannel == server->get_channelList()->end())
		{
			std::cout << "ERR_NOSUCHCHANNEL" << std::endl;
			send_response_message(client, ERR_NOSUCHCHANNEL, channelNames[i], server);
			continue ;
		}
		if (!client->is_in_channel(channelNames[i]))
		{
			std::cout << "ERR_NOTONCHANNEL" << std::endl;
			send_response_message(client, ERR_NOTONCHANNEL, channelNames[i], server);
			continue ;
		}
		std::cout << "Client::leave_channel()" << std::endl;
		client->leave_channel(currentChannel->second);
	}
	std::cout << "end of part_command()" << std::endl;
}