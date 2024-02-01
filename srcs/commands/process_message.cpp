/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 18:09:34 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/01 18:04:57 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include "server.hpp"
#include "utils.h"
#include "process_message.hpp"

enum e_commands
{
	CMD_CAP = 0,
	CMD_PASS,
	CMD_NICK,
	CMD_USER,
	CMD_WHOIS,
	CMD_PRIVMSG,
	CMD_JOIN,
	CMD_PART,
	CMD_KICK,
	CMD_MODE,
	CMD_INVITE,
	CMD_TOPIC,
	CMD_PING,
	CMD_PONG,
	CMD_QUIT
};

static std::vector<std::string>	tokenize_message(std::string message)
{
	std::vector<std::string> tokens;

	if (!message.empty())
		trim_whitespace(message);
	tokens = split(message, ' ');
	return tokens;
}

static int isRecognizedCommand(std::string command)
{
	static std::vector<std::string> commandList{"CAP", "PASS", "NICK", "USER", "WHOIS", "PRIVMSG", "JOIN", "PART", "KICK", "MODE", "INVITE", "TOPIC", "PING", "PONG", "QUIT"};
	size_t idx = 0;
	
	for (std::vector<std::string>::iterator iter = commandList.begin(); (iter + idx) != commandList.end(); idx++)
	{
		if (commandList[idx] == command)
			return idx;
	}
	return -1;
}

int process_message(Client *client, std::string message, Server *server)
{
	std::vector<std::string> tokens = tokenize_message(message);
	int	command = isRecognizedCommand(tokens[0]);
	std::cout << "after check for command" << std::endl;

	switch (command)
	{
		case CMD_CAP:
		{
			capabilities(client->get_fd(), tokens, server);
			break ;
		}
		case CMD_PASS:
		{
			client->set_correctPassword(tokens[1]);
			break ;
		}
		case CMD_NICK:
		{
			client->set_nickname(tokens[1]);
			break ;
		}
		case CMD_USER:
		{
			std::cout << "USER COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_WHOIS:
		{
			std::cout << "WHOIS COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_PRIVMSG:
		{
			std::cout << "PRIVMSG COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_JOIN:
		{
			std::cout << "JOIN COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_PART:
		{
			std::cout << "PART COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_KICK:
		{
			std::cout << "KICK COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_MODE:
		{
			std::cout << "MODE COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_INVITE:
		{
			std::cout << "INVITE COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_TOPIC:
		{
			std::cout << "TOPIC COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_PING:
		{
			std::cout << "PING COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_PONG:
		{
			std::cout << "PONG COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_QUIT:
		{
			std::cout << "QUIT COMMAND RECEIVED" << std::endl;
			break ;
		}
		default:
		{
			std::cout << "message sent to client." << std::endl;
			server->msg_to_client(client->get_fd(), message);
			break ;
		}
	}
	return 0;
}