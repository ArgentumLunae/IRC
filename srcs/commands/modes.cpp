/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   modes.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/28 16:41:43 by ahorling      #+#    #+#                 */
/*   Updated: 2024/03/31 13:43:03 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include "utils.h"
#include "process_message.hpp"


//ALL THE DIFFERENT MODE COMMANDS GO HERE

std::string	toggleInviteOnly(char toggle, Channel* channel)
{
	if (toggle == '+' && channel->get_inviteStatus() == true)
		return "";
	else if (toggle == '+' && channel->get_inviteStatus() == false)
	{
		channel->set_modes(MODE_INV);
		return ("+i");
	}
	else if (toggle == '-' && channel->get_inviteStatus() == true)
	{
		channel->unset_mode(MODE_INV);
		return ("-i");
	}
	else if (toggle == '-' && channel->get_inviteStatus() == false)
		return "";
	return "";
}

std::string	toggleTopicRestriction(char toggle, Channel* channel)
{
	if (toggle == '+' && channel->get_topicStatus() == true)
		return "";
	else if (toggle == '+' && channel ->get_topicStatus() == false)
	{
		channel->set_modes(MODE_TOP);
		return ("+t");
	}
	else if (toggle == '-' && channel ->get_topicStatus() == true)
	{
		channel->unset_mode(MODE_TOP);
		return ("-t");
	}
	else if (toggle == '-' && channel ->get_topicStatus() == false)
		return "";
	return "";
}

std::string	changePassword(char toggle, std::string newPass, Channel* channel)
{
	if (toggle == '-')
	{
		channel->unset_mode(MODE_KEY);
		channel->set_password("");
		return ("-k");
	}
	else if (toggle == '+' && newPass.empty())
		return ("");
	else if (toggle == '+' && !newPass.empty())
	{
		channel->set_modes(MODE_KEY);
		channel->set_password(newPass);
		return ("+k");
	}
	return "";
}

std::string	setUserLimit(char toggle, std::string newLimit, Channel* channel)
{
	if (toggle == '-')
	{
		channel->set_limit((int)channel->get_server()->get_config().get_maxClients());
		return ("-l");
	}
	else if (toggle == '+' && newLimit.empty())
		return ("");
	else if (toggle == '+' && !newLimit.empty())
	{
		int limit = 0;
		try {
			limit = std::stoi(newLimit);
		}
		catch (...) {
			return "";
		}
		//check to see if it is an actual number
		if (limit <= 0 || limit > (int)channel->get_server()->get_config().get_maxClients())
			return ("");
		channel->set_limit(limit);
		return ("+l");
	}
	return "";
}

std::string	 promoteOperator(char toggle, std::string toPromote, Channel* channel, Client* client, Server* server)
{
	Client* target = nullptr;
	std::vector<Client*> clientList = channel->get_clients();

	for (size_t j = 0; j < clientList.size(); j++)
	{
		if (clientList[j]->get_nickname() == toPromote)
		{
			target = clientList[j];
			break;
		}
	}
	if (target == nullptr)
	{
		send_response_message(client, ERR_USERNOTINCHANNEL, toPromote + " " + channel->get_name(), server);
		return "";
	}

	if (toggle == '-')
	{
		channel->remove_operator(target);
		return "-o";
	}
	else if (toggle == '+' && channel->client_is_operator(target) >= 0)
		return "";
	else if (toggle =='+' && channel->client_is_operator(target) < 0)
	{
		channel->add_operator(target);
		return "+o";
	}
	return "";
}

//END COMMAND FUNCTIONS
static bool contains_flag(char flag, std::vector<std::pair<char, char>> modesList)
{
	for (std::vector<std::pair<char, char>>::iterator iter = modesList.begin(); iter != modesList.end(); iter++)
	{
		if (iter->first == flag)
			return true;
	}
	return false;
}

std::vector<std::pair<char, char>>	split_modes(std::string tokens)
{
	std::vector<std::pair<char, char>> modesList;
	char sign = '0';
	char flag = '0';

	//if the first symbol is neither a + or -, send back an empty map
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == '+' || tokens[i] == '-')
		{
			sign = tokens[i];
			flag = '0';
		}
		else
			flag = tokens[i];
		if (sign != '0' && flag != '0' && !contains_flag(flag, modesList))
		{
			modesList.push_back(std::make_pair(flag, sign));
		}
	}
	return (modesList);
}


void	change_mode(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::pair<char, char>> modesList;
	std::string modePrefix;
	std::string modeChanges;

	//If there are only 2 passed paramaters in the command, send the current list of modes. if channel send channel commands
	if (tokens.size() < 2 || (tokens[1][0] != '#' && tokens[1][0] != '&'))
		return ;
	if (tokens.size() == 2 || (tokens.size() == 3 && tokens[3] == ":"))
	{
		server->msg_to_client(client->get_fd(), ":" + client->get_nickname() + "!" + client->get_hostmask() + " MODE " + tokens[1] + " :" + server->get_channel(tokens[1])->string_modes());
		return;
	}

	//check to see if stated channel exists, and if they have privs in that channel.
	std::map<std::string, Channel*>::iterator currentChannel;

	if (server->get_channelList()->empty())
		return send_response_message(client, ERR_NOSUCHCHANNEL, tokens[1], server);
	currentChannel = server->get_channelList()->find(tokens[1]);
	if (currentChannel == server->get_channelList()->end())
		return send_response_message(client, ERR_NOSUCHCHANNEL, tokens[1], server);
	if (currentChannel->second->client_is_operator(client) < 0)
		return send_response_message(client, ERR_CHANOPRIVSNEEDED, tokens[1], server);

	int count = 0;
	if (tokens.size() >= 3)
	{

		modesList = split_modes(tokens[2]);
		if (modesList.empty())
			return;
		for (std::vector<std::pair<char, char>>::iterator modeitr = modesList.begin(); modeitr != modesList.end(); ++modeitr)
		{
			switch (modeitr->first)
			{
				case 'i':
					modeChanges += toggleInviteOnly(modeitr->second, currentChannel->second);
					break;
				case 't':
					modeChanges += toggleTopicRestriction(modeitr->second, currentChannel->second);
					break;
				case 'k':
					if (modeitr->second == '-')
						modeChanges += changePassword(modeitr->second, "", currentChannel->second);
					else
					{
						if (tokens[3 + count] == *tokens.end())
							return;
						modeChanges += changePassword(modeitr->second, tokens[3 + count], currentChannel->second);
						count++;
					}
					break;
				case 'l':
					if (modeitr->second == '-')
						modeChanges += setUserLimit(modeitr->second, "", currentChannel->second);
					else
					{
						if (tokens[3 + count] == *tokens.end())
							return;
						modeChanges += setUserLimit(modeitr->second, tokens[3 + count], currentChannel->second);
						count++;
					}
					break;
				case 'o':
					if (tokens[3 + count] == *tokens.end())
						return;
					modeChanges += promoteOperator(modeitr->second, tokens[3 + count], currentChannel->second, client, server);
					count++;
					break;
				default:
					send_response_message(client, ERR_UNKNOWNMODE, {modeitr->first}, server);
			}
		}
	}

	//Once finished with tasks, send a response message to all clients with all the successfully sent mode changes
	std::string finalMessage = ":" + client->get_nickname() + "!" + client->get_hostmask();
	int i = 3;
	
	finalMessage += (" MODE " + tokens[1] + " :" + modeChanges);
	while(tokens[i] != *tokens.end() && i <= 3 + count)
	{
		finalMessage += (" " + tokens[i]);
		i++; 
	} 
	server->msg_to_client(client->get_fd(), finalMessage);
	currentChannel->second->msg_to_channel(client, finalMessage);
}
