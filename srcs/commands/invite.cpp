/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   invite.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/27 18:01:28 by ahorling      #+#    #+#                 */
/*   Updated: 2024/03/27 20:04:25 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"
#include "process_message.hpp"

void	invite_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::string invitedChannel;
	std::string invitee;
	std::string invitePrefix;

	if (tokens.size() < 2)
	{
		send_response_message(client, ERR_NEEDMOREPARAMS, tokens[1], server);
		return ;
	}
	invitee = tokens[1];
	invitedChannel = tokens[2];
	invitePrefix = ":" + client->get_nickname() + "!" + client->get_hostmask();
	
	//Check to make sure the channel exists, that inviter is in the channel and that the inviter is an operator if the channel is invite-only
	std::map<std::string, Channel*>::iterator currentChannel;

	if (server->get_channelList()->empty())
	{
		send_response_message(client, ERR_NOSUCHCHANNEL, invitedChannel, server);
		return;
	}
	currentChannel = server->get_channelList()->find(invitedChannel);
	if (currentChannel == server->get_channelList()->end())
	{
		send_response_message(client, ERR_NOSUCHCHANNEL, invitedChannel, server);
		return;
	}
	if (!client->is_in_channel(invitedChannel))
	{
		send_response_message(client, ERR_NOTONCHANNEL, invitedChannel, server);
		return;
	}
	if (currentChannel->second->get_inviteStatus() == true)
	{
		if (currentChannel->second->client_is_operator(client) < 0)
		{
			send_response_message(client, ERR_CHANOPRIVSNEEDED, invitedChannel, server);
			return;
		}
	}

//NEED TO GET THE SPECIFIC CLIENT POINTER THAT IS LINKED TO THE NICKNAME, BUT THE MAP IS KEYED WITH INTS NOOOOO

	std::map<std::string, Client*>::iterator invitedClient;
	

	for (size_t i = 0; i < server->get_clientList.size(); i++)
	{
		if (invitedClient[i]->get_nickname() == invitee)
		{
			invitedClient = clientList[i];
			break;
		}
	}
	if (invitedClient->second->is_in_channel(invitedChannel))
	{
		send_response_message(client, ERR_USERONCHANNEL, invitedChannel, server);
		return;
	}
}