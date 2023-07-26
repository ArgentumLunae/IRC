/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:40:14 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/26 14:05:35 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "Client.hpp"

/*-------- CONSTRUCTORS & DESTRUCTOR --------*/

Client::Client() {}

Client::Client(Client const & src)
{
	if (&src != NULL)
		*this = src;
}

Client::~Client() {}

/*-------- MEMBER FUNCTIONS --------*/

/*-------- OPERATORS --------*/

Client & Client::operator=(Client const & rhs)
{
	(void)rhs;
}

bool Client::operator==(Client const & rhs) const
{
	if (this->_userName == rhs.get_userName())
		return (true);
	return (false);
}

/*-------- GETTERS & SETTERS --------*/

std::string Client::get_userName(void) const {
	return (this->_userName);
}

void	Client::set_userName(std::string const newName) {
	this->_userName = newName;
}
