/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kick.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/26 17:37:48 by ahorling      #+#    #+#                 */
/*   Updated: 2024/03/30 16:21:31 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"
#include "process_message.hpp"

static	std::string	construct_kick_message(std::vector<std::string> & tokens)
{
	std::string kickMessage;

	if (tokens[3][0] != ':')
		return ":" + tokens[2];
	for (size_t i = 2; i < tokens.size(); i++)
		kickMessage += " " + tokens[i];
	return kickMessage;
}

void	kick_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> kickedNames;
	std::string	kickedChannel;
	std::string	kickMessage = "";
	std::string	kickPrefix;
	std::string	msg;

	if (tokens.size() < 3)
	{
		send_response_message(client, ERR_NEEDMOREPARAMS, tokens[1], server);
		return ;
	}
	kickedChannel = tokens[1];
	kickedNames = split(tokens[2], ',');
	if (tokens.size() > 3)
		kickMessage = construct_kick_message(tokens);
	kickPrefix = ":" + client->get_nickname() + "!" + client->get_hostmask();

	//Check to see if the channel to be kicked from exists, and if the kicker is an operator
	std::map<std::string, Channel*>::iterator currentChannel;

	if (server->get_channelList()->empty())
		return send_response_message(client, ERR_NOSUCHCHANNEL, kickedChannel, server);
	currentChannel = server->get_channelList()->find(kickedChannel);
	if (currentChannel == server->get_channelList()->end())
		return send_response_message(client, ERR_NOSUCHCHANNEL, kickedChannel, server);
	if (!client->is_in_channel(kickedChannel))
		return send_response_message(client, ERR_NOTONCHANNEL, kickedChannel, server);
	if (currentChannel->second->client_is_operator(client) < 0)
		return send_response_message(client, ERR_CHANOPRIVSNEEDED, kickedChannel, server);

	//remove each named client from the channel

	for (size_t i = 0; i < kickedNames.size(); i++)
	{
		Client* toKick = nullptr;
		std::vector<Client*> clientList = currentChannel->second->get_clients();

		for (size_t j = 0; j < clientList.size(); j++)
		{
			if (clientList[j]->get_nickname() == kickedNames[i])
			{
				toKick = clientList[j];
				break;
			}
		}

		if (toKick == nullptr)
		{
			std::string specifics = kickedNames[i] + " " + kickedChannel;
			send_response_message(client, ERR_USERNOTINCHANNEL, specifics, server);
			continue;
		}

		msg = kickPrefix + " KICK " + kickedNames[i] + " " + kickedChannel;
		if (!kickMessage.empty())
			msg += " " + kickMessage;
		currentChannel->second->kick(toKick, client);
		server->msg_to_client(client->get_fd(), kickPrefix + " KICK "+ kickedChannel + " "  + kickedNames[i] +  " :"+ kickMessage);
		currentChannel->second->msg_to_channel(client, kickPrefix + " KICK "+ kickedChannel + " "  + kickedNames[i] +  " :"+ kickMessage);
		if (server->get_channel(kickedChannel) != nullptr)
			server->msg_to_client(toKick->get_fd(), kickPrefix + " KICK "+ kickedChannel + " "  + kickedNames[i] +  " :"+ kickMessage);
	}
}