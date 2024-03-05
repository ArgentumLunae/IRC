/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quit.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/02 15:01:22 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/05 14:00:38 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	quit_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	std::string quitPrefix = ":" + client->get_nickname() + "!" + client->get_hostmask() + " QUIT :";
	std::string quitMessage = "Quit :";
	std::vector<Channel*> channelList = client->get_channelList();

	if (client->is_registered())
	{
		if (tokens.size() > 1)
		{
			quitMessage += tokens[1];
			if (quitMessage[0] != ':')
				quitMessage += ":";
			else
			{
				for (size_t i = 2; i < tokens.size(); i++)
					quitMessage += " " + tokens[i];
			}
		}
		for (std::vector<Channel*>::iterator iter = channelList.begin(); iter != channelList.end(); iter++)
			(*iter)->msg_to_channel(client, quitPrefix + quitMessage);
	}
	server->client_disconnect(client->get_fd());
}