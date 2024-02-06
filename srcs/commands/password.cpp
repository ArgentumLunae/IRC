/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   password.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 16:11:56 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/06 17:52:15 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"

void validate_password(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2)
		send_response_message(client, ERR_NEEDMOREPARAMS, "PASS ", server);
	else if (client->is_registered())
		send_response_message(client, ERR_ALREADYREGISTRED, "", server);
	else
		client->set_correctPassword(tokens[1]);
}
