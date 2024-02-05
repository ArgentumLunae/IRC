/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   responseMessage.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 16:29:46 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/02/05 16:59:05 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_MESSAGE_HPP
# define RESPONSE_MESSAGE_HPP

void send_response_message(Client *client, int responsecode, std::string specifics, Server *server);

#endif
