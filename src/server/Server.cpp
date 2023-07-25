/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:40:43 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/25 17:30:56 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

/*-------- CONSTRUCTORS & DESTRUCTOR --------*/

Server::Server() {}

Server::Server(Server const & src)
{
	*this = src;
}

Server::~Server() {}

/*-------- MEMBER FUNCTIONS --------*/

/*-------- OPERATORS --------*/

Server & Server::operator=(Server const & rhs)
{
	(void)rhs;
}

/*-------- GETTERS & SETTERS --------*/

