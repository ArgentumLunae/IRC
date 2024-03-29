/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   join.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/09 15:37:20 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/29 19:29:00 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <server.hpp>
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "process_message.hpp"
#include "utils.h"

static bool	pass_channel_modes(Client *client, Channel *channel, Server *server)
{
	if (channel->get_modes() & MODE_LIM && channel->get_clients().size() >= channel->get_userlimit())
	{
		send_response_message(client, ERR_CHANNELISFULL, channel->get_name(), server);
		return false;
	}
	if (channel->get_modes() & MODE_INV && !channel->is_invited(client)) //Add a check to see if the client is on the invite list if JOIN is used alongside INVITE
	{
		send_response_message(client, ERR_INVITEONLYCHAN, channel->get_name(), server);
		return false;
	}
	return true;
}

static bool	check_channel_mask(Client *client, std::string channelName, Server *server)
{
	if (channelName.size() > 200 || (channelName[0] != '#' && channelName[0] != '&') || channelName.find(' ') != std::string::npos || channelName.find(',')  != std::string::npos)
	{
		send_response_message(client, ERR_BADCHANMASK, channelName, server);
		return false;
	}
	return true;
}

static bool check_channel_key(Client *client, Channel *channel, std::string key, Server *server)
{
	std::cout << "\t" << (channel->get_modes() & MODE_KEY) << " | " << (!channel->get_password().empty()) << " | " << key << " == " << channel->get_password() << std::endl;

	if (channel->get_modes() & MODE_KEY && !channel->get_password().empty() && key != channel->get_password())
	{
		send_response_message(client, ERR_BADCHANNELKEY, "", server);
		return false;
	}
	return true;
}

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
		if (check_channel_mask(client, *iter, server) == false)
			continue ;
		if (server->add_channel(*iter, *client) != SUCCESS)
			return ;
		else
			currentChannel = server->get_channel(*iter);
		if (pass_channel_modes(client, currentChannel, server) == false)
			continue ;
		if (idx < channelKeys.size()) {
			if (check_channel_key(client, currentChannel, channelKeys[idx], server) == false)
				continue ;
		}
		else if(check_channel_key(client, currentChannel, "", server) == false)
			continue ;
		if (!currentChannel->add_client(client))
		{
			if (currentChannel->get_topic().empty())
				topic_command(client, std::vector<std::string>{"TOPIC", *iter}, server);
			else
				topic_command(client, std::vector<std::string>{"TOPIC", *iter, currentChannel->get_topic()}, server);
		}
		idx++;
		list_names(client, std::vector<std::string>{"NAMES", *iter}, server);
	}
}