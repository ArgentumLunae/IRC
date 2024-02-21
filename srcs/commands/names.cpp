/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   names.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/14 18:06:52 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/21 15:21:46 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "utils.h"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include <sstream>

static std::string	string_nicknames(std::vector<Client*> clientList, Channel *channel)
{
	std::string nickList("");
	std::string prefix = "";

	// sstream << channel->get_name() << " :";
	for (std::vector<Client*>::iterator iter = clientList.begin(); iter != clientList.end(); iter++)
	{
		if (iter != clientList.begin())
			nickList += " ";
		if (channel->client_is_operator(*iter))
			prefix = "@";
		else
			prefix = "";
		nickList += prefix + (*iter)->get_nickname();
	}
	std::cout << nickList << std::endl;
	return nickList;
}

void list_names(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::vector<std::string> channelList;
	std::string	nameReply;
	Channel *currentChannel;

	if (tokens.size() < 2)
		return ;
	std::cout << "list_names()" << std::endl;
	channelList = split(tokens[1], ',');
	for (std::vector<std::string>::iterator iter = channelList.begin(); iter != channelList.end(); iter++)
	{
		std::stringstream sstream;
		std::cout << "currentChannel == " << *iter << std::endl;
		currentChannel = server->get_channel(*iter);
		if (currentChannel != nullptr)
		{
			std::vector<Client*> clientList = currentChannel->get_clients();

			sstream << ":" << server->get_config().get_host() << " " << RPL_NAMREPLY << " " << client->get_nickname() << " @ " << *iter << " :";
			server->msg_to_client(client->get_fd(),  sstream.str() + string_nicknames(currentChannel->get_clients(), currentChannel) + "\r\n");
		}
		sstream.str("");
		sstream << ":" << server->get_config().get_host() + " " << RPL_ENDOFNAMES << " " << client->get_nickname() << " " << *iter << " :\r\n";
		std::cout << sstream.str() << std::endl;
		server->msg_to_client(client->get_fd(), sstream.str());
	}
}