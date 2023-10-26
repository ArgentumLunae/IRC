#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include "client.hpp"
# include "channel.hpp"
# include "config.hpp"

class Server
{
	private:
		Config		_config;
        std::string _name;
		std::string	_pass;
		int			_port;
		std::map<int, Client> 			_clientList;
		std::map<std::string, Channel>	_channelList;

	public:
		//Constructors/deconstructors
		Server(std::string pass, int port);
		~Server();
		
		// Getters
		Config		get_config() const;
		std::string	get_name() const;
		std::string	get_pass() const;
		int			get_port() const;
		std::map<int, Client>* get_clientList();
		Client*		get_client(int fd); //get a client using their fd
		Client*		get_client(std::string nickname); //get a client by using their nickname
		std::map<std::string, Channel>* get_channelList();
		Channel*	get_channel(std::string channelName);

        //Setters

        //Others
		int 	add_client(int fd);
		int	    remove_client(int fd);
		bool	nickname_in_use(std::string nickname);
		int 	add_channel(std::string channelName, Client &client);
		int	    remove_channel(std::string channelName);
		int		start_server();
};

#endif