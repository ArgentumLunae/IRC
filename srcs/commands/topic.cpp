/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   topic.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/28 12:20:11 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/29 19:47:12 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"
#include <sstream>

static void	change_topic(Client *client, Channel *channel, std::string newTopic, Server *server)
{
	std::string reply;

	std::cout << (channel->get_modes() & MODE_TOP) << std::endl;
	if ((channel->get_modes() & MODE_TOP) && channel->client_is_operator(client) < 0)
		return send_response_message(client, ERR_CHANOPRIVSNEEDED, channel->get_name(), server);
	channel->set_topic(newTopic);
	reply = ":" + client->get_nickname() + "!" + client->get_hostmask() + " TOPIC " + channel->get_name() + " " + newTopic;
	server->msg_to_client(client->get_fd(), reply);
	channel->msg_to_channel(client, reply);
}

static void display_topic(Client *client, Channel *channel, Server *server)
{
	std::stringstream reply;
	std::string topic = channel->get_topic();

	reply << ":" << server->get_config().get_host() << " ";

	if (topic.empty())
		reply << RPL_NOTOPIC << " " << channel->get_name() << " :\x1b[35mNo topic is set\x1b[0m";
	else
		reply << RPL_NOTOPIC << " "  << channel->get_name() << " \x1b[34m" << topic << "\x1b[0m";
	server->msg_to_client(client->get_fd(), reply.str());
}

void	topic_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	Channel *channel;

	if (tokens.size() < 2)
		return send_response_message(client, ERR_NEEDMOREPARAMS, "TOPIC", server);
	channel = client->get_channel(tokens[1]);
	if (!client->is_in_channel(tokens[1]) || channel == nullptr)
		return send_response_message(client, ERR_NOTONCHANNEL, ":" + tokens[1], server);
	if (tokens.size() > 2)
	{
		std::string newTopic = "";
		if (tokens[2][0] != ':')
			newTopic += ":" + tokens[2];
		else
		{
			newTopic += tokens[2];
			for (size_t i = 3; i < tokens.size(); i++)
				newTopic += " " + tokens[i];
		}
		change_topic(client, channel, newTopic, server);
	}
	else
		display_topic(client, channel, server);
}
