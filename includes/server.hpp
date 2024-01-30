#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <poll.h>
# include <netinet/in.h>
# include "client.hpp"
# include "channel.hpp"
# include "config.hpp"

# define SUCCESS 0
# define FAILURE 1

class Server
{
	private:
		Config		_config;
        std::string _name;
		std::string	_pass;
		int			_port;
		std::map<int, Client> 			_clientList;
		std::map<std::string, Channel>	_channelList;

		std::vector<pollfd>	_fds;	// Socket file descriptors currently in use
		int					_nfds;	// Number of socket file descriptors currently in use
		int					_serverSocket;
		struct sockaddr_in	_serverAddr;


		int initServer();
		void runServer();
		void checkRevents();
		int clientConnect(size_t idx);
		void clientDisconnect(size_t idx);
		int incomingData(size_t idx);
		int outgoingData(int clientfd);
		void closeServer();

		void increment_nfds();
		void decrement_nfds();

	public:
		//Constructors/deconstructors
		Server(std::string setpass, int setport);
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
		int		msg_to_client(int clientfd, std::string msg);
};

#endif