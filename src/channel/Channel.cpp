/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:36:22 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/26 14:11:18 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "Channel.hpp"

/*-------- CONSTRUCTORS & DESTRUCTOR --------*/

Channel::Channel() {}

Channel::Channel(std::string const name, std::string const key, std::string const topic) : _channelName(name), _channelKey(key), _channelTopic(topic)
{
}

Channel::Channel(Channel const & src)
{
	if (&src != NULL)
		*this = src;
}

Channel::~Channel() {}

/*-------- MEMBER FUNCTIONS --------*/

bool	Channel::isMember(Client const & client)
{
	std::vector<Client>::iterator iter = this->_members.begin();
	std::vector<Client>::iterator iterEnd = this->_members.end();

	for (iter; iter != iterEnd; iter++)
	{
		if (client == *(iter.base()))
			return (true);
	}
	return (false);
}

bool	Channel::isOperator(Client const & member)
{
	std::vector<Client>::iterator iter = this->_operators.begin();
	std::vector<Client>::iterator iterEnd = this->_operators.end();

	for (iter; iter != iterEnd; iter++)
	{
		if (member == *(iter.base()))
			return (true);
	}
	return false;
}

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

std::string	Channel::get_channelKey(void) const {
	return (this->_channelKey);
}

void	Channel::set_channelKey(std::string const newKey) {
	this->_channelKey = newKey;
}

int	Channel::get_userLimit(void) const {
	return (this->_userLimit);
}

void	Channel::set_userLimit(int const newLimit) {
	this->_userLimit = newLimit;
}

std::vector<Client>	Channel::get_members(void) const {
	return (this->_members);
}

std::vector<Client> Channel::get_operators(void) const {
	return (this->_operators);
}
