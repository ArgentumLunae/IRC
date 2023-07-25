/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:33:07 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/25 17:45:38 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ircserv.hpp"

class Client 
{
	private:
		std::string _nickName;
		std::string	_userName;
		std::vector<Channel> _subscribedChannels;

	public:
		Client();
		Client(Client const & src);
		~Client();

		Client & operator=(Client const & rhs);

		std::string	get_nickName(void) const;
		void	set_nickName(std::string const newNick);
		std::string get_userName(void) const;
		void	set_userName(std::string const newUser);
		
		void	joinChannel(Channel & channel);
};

#endif
