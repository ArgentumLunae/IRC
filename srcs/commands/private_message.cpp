/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   private_message.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/13 13:01:29 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/15 15:47:41 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "responseMessage.hpp"
#include "responsecodes.hpp"

void	private_message(Client *client, std::vector<std::string> tokens, Server *server)
{
	if (tokens.size() < 2)
		send_response_message(client, ERR_NORECIPIENT, "", server);
	else if (tokens.size() < 3)
		send_response_message(client, ERR_NOTEXTTOSEND, "", server);
	
}