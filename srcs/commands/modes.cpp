/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   modes.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/28 16:41:43 by ahorling      #+#    #+#                 */
/*   Updated: 2024/03/28 20:17:53 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"
#include "process_message.hpp"

char	validinput(char input)
{
	if (input != '-' && input != '+')
		return ('0');
	
	if (input != 't' && input != 'k' && input != 'l' && input != 'i' && input != 'o')
		return (input);
}

std::map<char, char>	split_modes(std::string tokens)
{
	std::map<char, char> modesList;
	char sign = '0';
	char flag = '0';

	//if the first symbol is neither a + or -, send back an empty map
	for (int i = 0; i < tokens.size(); i++)
	{
		if (validinput(token[i]) == '0')
			return nullptr;
		if (token[i] == "+" || token[i] == "-")
		{
			sign = token[i];
			flag = '0';
		}
		else if (token[i] != 't' || token[i] != 'k' || token[i] != 'l' || token[i] != 'i' || token[i] != 'o')
			flag = token[i];
		if (sign != '0' && flag != '0')
			modesList[flag].insert(sign);
	}
	return (modesList);
}


void	change_mode(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::map<char, char> modesList;

	//If there are only 2 passed paramaters in the command, send the current list of modes. if channel send channel commands
	if (tokens.size() == 2)
	{
		server->msg_to_client(client->get_fd(), server->get_config()->get_channelModes());
	}

	//check to see if stated channel exists, and if they have privs in that channel.
	std::map<std::string, Channel*>::iterator currentChannel;

	if (server->get_channelList()->empty())
	{
		send_response_message(client, ERR_NOSUCHCHANNEL, token[1], server);
		return;
	}
	currentChannel = server->get_channelList()->find(token[1]);
	if (currentChannel == server->get_channelList()->end())
	{
		send_response_message(client, ERR_NOSUCHCHANNEL, token[1], server);
		return;
	}
	if (currentChannel->second->client_is_operator(client) < 0)
	{
		send_response_message(client, ERR_CHANOPRIVSNEEDED, token[1], server);
		return;
	}

	if (tokens.size() == 3)
	{
		modesList = split_modes(tokens[2]);
		if (modesList = nullptr)
			return;
		for (std::map<char, char> modeitr = modesList.begin(); modeitr != clientList.end(); ++modeitr)
		{
			if (modeitr->first == "i")

		}
	}
}
