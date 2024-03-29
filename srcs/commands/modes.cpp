/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   modes.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ahorling <ahorling@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/28 16:41:43 by ahorling      #+#    #+#                 */
/*   Updated: 2024/03/29 19:28:54 by ahorling      ########   odam.nl         */
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
		channel->set_inviteOnly(true);
		channel->set_modes(MODE_INV);
		return ("+i");
	}
	else if (toggle == '-' && channel->getinviteStatus() == true)
	{
		channel->set_inviteOnly(false);
		channel->unset_modes(MODE_INV);
		return ("-i");
	}
	else if (toggle == '-' && chanel->getinviteStatus() == false)
		return "";
}

std::string	toggleTopicRestriction(char toggle, Channel* channel)
{
	if (toggle == '+' && channel->get_topicStatus() == true)
		return "";
	else if (toggle == '+' && channel ->get_topicStatus() == false)
	{
		channel->set_topicStatus(true);
		channel->set_modes(MODE_TOP);
		return ("+t");
	}
	else if (toggle == '-' && channel ->get_topicStatus() == true)
	{
		channel->set_topicStatus(false);
		channel->unset_modes(MODE_TOP);
		return ("-t");
	}
	else if (toggle == '-' && channel ->get_topicStatus() == false)
		return "";
}

std::string	changePassword(char toggle, std::string newPass, Channel* channel)
{
	if (toggle == '-')
	{
		// MAKE SURE TO SET PASSWORD MODE
		channel->unset_mode(MODE_KEY);
		channel->set_password("");
		return ("-k");
	}
	else if (toggle == '+' && newPass == "")
		return "";
	else if (toggle == '+' && newPass != "")
	{
		channel->set_mode(MODE_KEY);
		channel->setpassword(newPass);
		return ("+k");
	}
}

std::string	setUserLimit(char toggle, std::string newLimit, Channel* channel)
{
	if (toggle == '-')
	{
		channel->set_limit(channel->get_server()->get_config().get_maxClients());
		return ("-l");
	}
	else if (toggle == '+' && newLimit == "")
		return;
	else if (toggle == '+' && newLimit != "")
	{
		try
			int limit = std::stoi(newLimit);
		catch(const std::exception& e)
			return;
		if (limit < 0 || limit > channel->get_server()->get_config().get_maxClients())
			return;
		channel->set_limit(limit);
		return ("+l");
	}

}

std::string	 promoteOperator(char toggle, std::string toPromote, Channel* channel)
{
	Client* target = nullptr;
	std::vector<Client*> clientList = currentChannel->second->get_clients();

	for (size_t j = 0; j < clientList.size(); j++)
	{
		if (clientList[j]->get_nickname() == kickedNames[i])
		{
			target = clientList[j];
			break;
		}
	}
	if (target == nullptr)
	{
		send_response_message(client, ERR_USERNOTINCHANNEL, toPromote + " " + channel->get_name(), server);
		continue;
	}

	if (toggle == '-')
	{
		channel->remove.operator(target);
		return ("-o");
	}
	else if (toggle == '+' && channel->client_is_operator(target) >= 0)
		return;
	else if (toggle =='+' && channel->client_is_operator(target < 0))
	{
		channel->add_operator(target);
		return ("+o");
	}
}

//END COMMAND FUNCTIONS

std::map<char, char>	split_modes(std::string tokens)
{
	std::map<char, char> modesList;
	char sign = '0';
	char flag = '0';

	//if the first symbol is neither a + or -, send back an empty map
	for (int i = 0; i < tokens.size(); i++)
	{
		if (token[i] == "+" || token[i] == "-")
		{
			sign = token[i];
			flag = '0';
		}
		else
			flag = token[i];
		if (sign != '0' && flag != '0')
			modesList[flag].insert(sign);
	}
	return (modesList);
}


void	change_mode(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::map<char, char> modesList;
	std::string modePrefix;
	std::string modeChanges;

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

	// if (tokens.size() == 3)
	// {
	// 	modesList = split_modes(tokens[2]);
	// 	if (modesList.empty())
	// 		return;
	// 	for (std::map<char, char>::iterator modeitr = modesList.begin(); modeitr != clientList.end(); ++modeitr)
	// 	{
	// 		switch (modeitr->first)
	// 		{
	// 			case "i":
	// 				toggleInviteOnly(modeitr->second, currentChannel->second);
	// 				break;
	// 			case "t":
	// 				toggleTopicRestriction(modeitr->second, currentChannel->second);
	// 				break;
	// 			case "k":
	// 				changePassword(modeitr->second, nullptr, currentChannel->second);
	// 				break;
	// 			case "l":
	// 				setUserLimit(modeitr->second, nullptr, currentChannel->second);
	// 				break;
	// 			case "o":
	// 				promoteOperator(modeitr->second, nullptr, currentChannel->second);
	// 				break;
	// 			default:
	// 				send_response_message(client, ERR_UNKNOWNMODE, modeitr->first);
	// 		}
	// 	}
	// 	return;
	// }

	if (tokens.size() >= 3)
	{
		int count = 0;

		modesList = split_modes(tokens[2]);
		if (modesList.empty())
			return;
		for (std::map<char, char>::iterator modeitr = modesList.begin(); modeitr != clientList.end(); ++modeitr)
		{
			switch (modeitr->first)
			{
				case "i":
					modeChanges += toggleInviteOnly(modeitr->second, currentChannel->second);
					break;
				case "t":
					modeChanges += toggleTopicRestriction(modeitr->second, currentChannel->second);
					break;
				case "k":
					if (modeitr->second == '-')
						modeChanges += changePassword(modeitr->second, "", currentChannel->second);
					else
					{
						if (!token[3 + count])
							return;
						modeChanges += changePassword(modeitf->second, token[3 + count], currentChannel->second);
						count++;
					}
					break;
				case "l":
					if (modeitr->second == '-')
						modeChanges += setUserLimit(modeitr->second, "", currentChannel->second);
					else
					{
						if (!token[3 + count])
							return;
						modeChanges += setUserLimit(modeitr->second, token[3 + count], currentChannel->second);
						count++;
					}
					break;
				case "o":
					if (!token[3 + count])
						return;
					modeChanges += promoteOperator(modeitr->second, token[3 + count], currentChannel->second);
					count++;
					break;
				default:
					send_response_message(client, ERR_UNKNOWNMODE, modeitr->first);
			}
		}
	}

	//Once finished with tasks, send a response message to all clients with all the successfully sent mode changes
	std::string finalMessage = ":" + client->get_nickname() + "!" + client->get_hostmask();
	int i = 3
	
	finalMessage += (" MODE " + modeChanges);
	while(tokens[i])
	{
		finalMessage += (" " + tokens[i]);
		i++; 
	} 
	server->msg_to_client(client->get_fd(), finalMessage);
	currentChannel->second->msg_to_channel(client->get_fd(), finalMessage);
}
