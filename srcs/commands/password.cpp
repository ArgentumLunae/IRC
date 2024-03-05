/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   password.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 16:11:56 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/02 18:37:38 by mteerlin      ########   odam.nl         */
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
	else if (server->get_pass() != tokens[1])
		send_response_message(client, ERR_PASSWDMISMATCH, "", server);
	else
		client->set_correctPassword(tokens[1]);
}
