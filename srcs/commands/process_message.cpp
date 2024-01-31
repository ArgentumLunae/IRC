/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 18:09:34 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/31 17:03:09 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include "utils.h"

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

	for (size_t i = 0; commandList[i] != *(commandList.end()); i++)
	{
		if (commandList[i] == command)
			return i;
	}
	return -1;
}

int process_message(std::string message)
{
	std::vector<std::string> tokens = tokenize_message(message);
	int	command = isRecognizedCommand(tokens[0]);

	switch (command)
	{
		case CMD_CAP:
		{
			std::cout << "CAP COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_PASS:
		{
			std::cout << "PASS COMMAND RECEIVED" << std::endl;
			break ;
		}
		case CMD_NICK:
		{
			std::cout << "NICK COMMAND RECEIVED" << std::endl;
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
			break ;
	}
	return 0;
}