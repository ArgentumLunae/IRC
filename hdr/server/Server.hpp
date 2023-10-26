/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:35:13 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/28 17:04:17 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ircserv.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <vector>

class Server
{
	private:
		std::vector<struct pollfd>	_fds;
		std::string					_serverName;
		std::string					_password;
		int							_clientLimit;

		std::map<int, Client>			_clientsByConnection;
		std::map<std::string, Channel>	_channelsByKey;

	public:
		Server();
		Server(int socketfd, std::string name, std::string password)
		Server(Server const & src);
		~Server();

		Server & operator=(Server const & rhs);

		std::vector<struct pollfd>		get_serverfd(void) const;
		void	set_serverfd(int const newfd);
		std::string	get_serverName(void) const;
		void	set_serverName(std::string const newName);
		std::string	get_password(void) const;
		void	set_password(std::string const newPass);


		std::map<int, Client>	get_clientsByConnection(void) const;
		void	set_clientsByConnection(std::map<int, Client> clientMap);
		std::map<std::string, Channel> get_channelsByKey(void) const;
		void	set_channelsByKey(std::map<int, Channel> channelMap);
};

#endif
