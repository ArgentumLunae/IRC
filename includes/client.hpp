#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include "channel.hpp"

class Server;
class Channel;

class Client
{
	private:
		int			_fd;
		std::string	_nickname;
		std::string	_username;
		std::string _hostname;
		std::string	_hostmask;
		std::string	_fullname;
		bool		_correctPassword;
		bool		_capabilityNegotiation;
		bool		_registered;
		Server		*_server;
		std::vector<Channel *> _channelList;
		std::vector<Channel *> _partedChannels;
		std::deque<std::string> _incomingMessages;
		std::string				_messageBuffer;

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
		std::string get_hostname() const;
		std::string get_hostmask() const;
		std::string	get_fullname() const;
		bool		get_correctPassword() const;
		bool		get_isRegistred() const;
		bool		get_capabilityNegotiation() const;
		Server*		get_server() const;
		std::vector<Channel*> get_channelList(void) const;
		Channel*	get_channel(std::string channelName);
		std::string	get_messageBuffer() const;

		//Setters
		int			set_nickname(std::string const nickname);
		int			set_username(std::string const username);
		void		set_hostname(std::string const hostname);
		void		set_hostmask(std::string const hostmask);
		int			set_correctPassword(std::string password);
		void		set_capabilityNegotiation(bool state);
		void		set_registered(bool const state);

		//Others
		int			add_channel(Channel *channel);
		int			join_channel(std::string channelName, std::string password);
		int			leave_channel(Channel *channel);
		void		leave_all_channels(void);
		bool		is_in_channel(std::string channelName);

		void	push_message(std::string message);
		std::string	pop_message(void);
		bool	has_incoming_messages(void) const;
		void	add_to_message_buffer(std::string partialMessage);
		void	clear_message_buffer(void);
		bool	is_registered(void) const;
};

#endif