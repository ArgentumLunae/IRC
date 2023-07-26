/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:21:15 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/26 14:11:08 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "ircserv.hpp"
# include <vector>

class Channel 
{
	private:
		std::string	_channelName;
		std::string _channelKey;	//password

		int		_userLimit;
		bool	_inviteOnly;

		std::string _channelTopic;
		std::string _channelMode;

		std::vector<Client> _members;
		std::vector<Client> _operators;

		bool	isMember(Client const & client);
		bool	isOperator(Client const & member);

	public:
		Channel();
		Channel(std::string const name, std::string const key, std::string const topic);
		Channel(Channel const & src);
		~Channel();

		Channel & operator=(Channel const & rhs);

		std::string get_channelName(void) const;
		void	set_channelName(std::string const newName);
		std::string get_channelKey(void) const;
		void	set_channelKey(std::string const newKey);

		int		get_userLimit(void) const;
		void	set_userLimit(int const newLimit);

		std::vector<Client>	get_members(void) const;
		// void	set_members(std::vector<Client> const & newList); // likely unncessary
		std::vector<Client>	get_operators(void) const;
		// void	set_operators(std::vector<Client> const & newList); // likely unncessary


		//The following is a list of functionalities that are expected
		// void	kickMember();
		// void	inviteMember();
		// void	changeTopic();
		// void	viewTopic();
		// void	setInviteOnly();
		// void	removeInviteOnly();
		// void	setTopicRestriction();
		// void	removeTopicRestriction();
		// void	setChannelKey();
		// void	removeChannelKey();
		// void	makeOperator();
		// void	removeOperator();
		// void	setUserLimit();
		// void	removeUserLimit();
};

#endif
