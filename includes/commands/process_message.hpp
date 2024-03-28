/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_message.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/31 16:56:07 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/03/27 18:02:21 by ahorling      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_MESSAGE_HPP
# define PROCESS_MESSAGE_HPP
# include <string>
# include <vector>
# include "server.hpp"

int 	process_message(Client *client, std::string message, Server *server);
void 	capabilities(Client *client, std::vector<std::string> tokens, Server *server);
void 	validate_password(Client *client, std::vector<std::string> tokens, Server *server);
void	register_nickname(Client *client, std::vector<std::string> tokens, Server *server);
void	register_username(Client *client, std::vector<std::string> tokens, Server *server);

void	join_command(Client *client, std::vector<std::string> tokens, Server *server);
void	part_command(Client *client, std::vector<std::string> tokens, Server *server);
void	list_names(Client *client, std::vector<std::string> tokens, Server *server);
void	topic_command(Client *client, std::vector<std::string> tokens, Server *server);
void	kick_command(Client *client, std::vector<std::string> tokens, Server *server);
void	invite_command(Client *client, std::vector<std::string> tokens, Server *server);

void	private_message(Client *client, std::vector<std::string> tokens, Server *server);

void	ping_command(Client *client, std::vector<std::string> tokens, Server *server);
void	pong_command(Client *client, std::vector<std::string> tokens, Server *server);

void	quit_command(Client *client, std::vector<std::string> tokens, Server *server);

#endif