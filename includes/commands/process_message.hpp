/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/31 16:56:07 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/01 16:19:35 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_MESSAGE_HPP
# define PROCESS_MESSAGE_HPP
# include <string>
# include <vector>
# include "server.hpp"

int process_message(Client *client, std::string message, Server *server);
void capabilities(int clientfd, std::vector<std::string> tokens, Server *server);

#endif