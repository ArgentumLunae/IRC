/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 18:09:34 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/27 14:31:02 by ahorling      ########   odam.nl         */
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
	CMD_NAMES,
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
	static std::vector<std::string> commandList{"CAP", "PASS", "NICK", "USER", "NAMES", "PRIVMSG", "JOIN", "PART", "KICK", "MODE", "INVITE", "TOPIC", "PING", "PONG", "QUIT"};
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

	switch (command)
	{
		case CMD_CAP:
		{
			capabilities(client, tokens, server);
			break ;
		}
		case CMD_PASS:
		{
			validate_password(client, tokens, server);
			server->finish_client_registration(client);
			break ;
		}
		case CMD_NICK:
		{
			register_nickname(client, tokens, server);
			server->finish_client_registration(client);
			break ;
		}
		case CMD_USER:
		{
			register_username(client, tokens, server);
			server->finish_client_registration(client);
			break ;
		}
		case CMD_QUIT:
		{
			quit_command(client, tokens, server);
			return SUCCESS;
		}
	}
	if (client->is_registered())
	{
		switch (command)
		{
			case CMD_NAMES:
			{
				list_names(client, tokens, server);
				break ;
			}
			case CMD_PRIVMSG:
			{
				private_message(client, tokens, server);
				break ;
			}
			case CMD_JOIN:
			{
				join_command(client, tokens, server);
				break ;
			}
			case CMD_PART:
			{
				part_command(client, tokens, server);
				break ;
			}
			case CMD_KICK:
			{
				kick_command(client, tokens, server);
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
				topic_command(client, tokens, server);
				break ;
			}
			case CMD_PING:
			{
				ping_command(client, tokens, server);
				break ;
			}
			case CMD_PONG:
			{
				pong_command(client, tokens, server);
				break ;
			}
			default:
				return FAILURE;
		}
	}
	return SUCCESS;
}