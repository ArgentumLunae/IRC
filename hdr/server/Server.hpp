/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 16:35:13 by mteerlin      #+#    #+#                 */
/*   Updated: 2023/07/25 17:17:54 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <map>
#include "ircserv.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Server
{
	private:
		std::map<int, Client>	_clientsByConnection;
		std::map<std::string, Channel> _channelsByKey;

	public:
		Server();
		Server(Server const & src);
		~Server();

		Server & operator=(Server const & rhs);

		std::map<int, Client>	get_clientsByConnection(void) const;
		void	set_clientsByConnection(std::map<int, Client> clientMap);
		std::map<std::string, Channel> get_channelsByKey(void) const;
		void	set_channelsByKey(std::map<int, Channel> channelMap);
};

#endif
