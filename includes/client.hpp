#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <vector>
# include "channel.hpp"
# include "client.hpp"

class Server;

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string	_fullname;
		bool		_correctPassword;
		Server		*_server;
		std::vector<Channel*> _channelList;

	public:
		//Constructors/deconstructors
		Client(int fd, Server* server);
		Client(const Client &copy);
		~Client();
		Client	&operator=(const Client &copy);
		
        //Getters
		int			get_fd() const;
		std::string	get_nickname() const;
		std::string	get_username() const;
		std::string	get_fullname() const;
		bool		get_correctPassword() const;
		Server*		get_server() const;

		//Setters
		int			set_nickname(std::string nickname);
		int			set_username(std::string username);
		int			set_correctPassword(std::string password);

		//Others
		int			join_channel(std::string channelName, std::string password);
		int			leave_channel(std::string channelName);
		std::vector<Channel*> get_channelList(void);
		Channel*	get_channel(std::string channelName);
		bool		is_in_channel(std::string channelName) const;
};

#endif