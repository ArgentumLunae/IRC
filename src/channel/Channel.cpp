/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:36:22 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/25 17:26:32 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "Channel.hpp"

/*-------- CONSTRUCTORS & DESTRUCTOR --------*/

Channel::Channel() {}

Channel::Channel(Channel const & src)
{
	if (&src != NULL)
		*this = src;
}

Channel::~Channel() {}

/*-------- MEMBER FUNCTIONS --------*/

/*-------- OPERATORS --------*/

Channel & Channel::operator=(Channel const & rhs)
{
	(void)rhs;	
}

/*-------- GETTERS & SETTERS --------*/

std::string	Channel::get_channelName(void) const {
	return (this->_channelName);
}

void	Channel::set_channelName(std::string const newName) {
	this->_channelName = newName;
}
