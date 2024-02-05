/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   username.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 16:22:21 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/05 17:10:44 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responsecodes.hpp"
#include "responseMessage.hpp"
#include <sstream>

void register_username(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 5)
		send_response_message(client, ERR_NEEDMOREPARAMS, "", server);
	else
		client->set_username(tokens[1]);
}
