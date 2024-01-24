/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 18:09:34 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/24 19:41:01 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>

enum e_commands
{
	CMD_PASS = 0,
	CMD_NICK,
	CMD_USER,
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

bool isRecognizedCommand(std::string command)
{
	static std::vector<std::string> commandList{"PASS", "NICK", "USER", "JOIN", "PART", "KICK", "MODE", "INVITE", "TOPIC", "PING", "PONG", "QUIT"};

	for (size_t i = 0; commandList[i] != *(commandList.end()); i++)
	{
		if (commandList[i] == command)
			return (i);
	}
}