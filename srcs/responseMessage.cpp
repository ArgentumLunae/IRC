/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   responseMessage.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 16:56:16 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/06 17:02:00 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responsecodes.hpp"
#include <sstream>
#include <map>

#include <iostream>

// void err_need_more_parameters(Client *client, std::string command, Server *server)
// {
// 	std::stringstream errorMsg;

// 	errorMsg << ":" << server->get_config().get_host() << " " << ERR_NEEDMOREPARAMS << " :" << command << " :Not enough parameters";
// 	server->msg_to_client(client->get_fd(), errorMsg.str());
// }

// void err_already_registered(Client *client, std::string command, Server *server)
// {
// 	std::stringstream errorMsg;

// 	errorMsg << ":" << server->get_config().get_host() << " " << ERR_ALREADYREGISTRED << " :" << " :You may not reregister";
// 	server->msg_to_client(client->get_fd(), errorMsg.str());
// }

static std::map<int, std::string>	map_response_messages()
{
	std::map<int, std::string> map;

	map.insert(std::make_pair(ERR_NEEDMOREPARAMS, ":Not enough parameters"));
	map.insert(std::make_pair(ERR_ALREADYREGISTRED, ":You may not reregister"));

	map.insert(std::make_pair(ERR_NONICKNAMEGIVEN, ":No nickname given"));
	map.insert(std::make_pair(ERR_ERRONEUSENICKNAME, ":Erroneous nickname"));
	map.insert(std::make_pair(ERR_NICKNAMEINUSE, ":Nickname is already in use"));
	map.insert(std::make_pair(ERR_NICKCOLLISION, ":Nickname collision KILL"));

	return map;
}

void send_response_message(Client *client, int responsecode, std::string specifics, Server *server)
{
	static std::map<int, std::string> responses = map_response_messages();
	std::stringstream msg;

	msg << ":" << server->get_config().get_host() << " " << responsecode << " ";
	if (specifics.empty())
		msg << responses.at(responsecode) << "\r\n";
	else
		msg << ":" << specifics << responses.at(responsecode) << "\r\n";
	server->msg_to_client(client->get_fd(), msg.str());
}