/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pingpong.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 13:31:51 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/05 17:20:06 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"

void	ping_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2)
		return send_response_message(client, ERR_NEEDMOREPARAMS, "PING", server);
	if (tokens[1] == server->get_config().get_host())
		server->msg_to_client(client->get_fd(), "PONG :" + tokens[1]);
	else
		return send_response_message(client, ERR_NOSUCHSERVER, ":" + tokens[1], server);
}

void	pong_command(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2)
		return send_response_message(client, ERR_NEEDMOREPARAMS, "PONG", server);
	if (tokens[1] != server->get_config().get_host())
		return send_response_message(client, ERR_NOSUCHSERVER, ":" + tokens[1], server);
}