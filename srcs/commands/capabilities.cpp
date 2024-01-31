/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   capabilities.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/30 16:54:13 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/31 18:03:11 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

void capabilities(int clientfd, std::vector<std::string> tokens)
{
	if (tokens[2] == "LS")
		send(clientfd, "CAP * LS")
}