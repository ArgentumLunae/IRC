#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include "client.hpp"

class Server;
class Client;

enum modes {
	MODE_INV = 1,
	MODE_TOP = 2,
	MODE_KEY = 4,
	MODE_LIM = 8
};

class Channel
{
	private:
		std::string	_name;
		std::string	_topic;
		uint8_t		_modes;
		std::string	_password;
		Server*		_server;
		Client*		_owner;
		std::vector<Client*>	_clients;
		std::vector<Client*>	_partedClients;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_invitelist;
		size_t		_userlimit;

		bool	check_operator_priv(Client *client);

	public:
        //Constructors/Deconstructors
		Channel(std::string name, Client* creator, Server *server);
		Channel(const Channel &copy);
		~Channel();
		Channel	&operator=(const Channel &copy);

		//Getters
		std::string				get_name() const;
		std::string				get_topic() const;
		// bool					get_topic_operator() const;
		std::string				get_password() const;
		uint8_t					get_modes() const;
		Server*					get_server() const;
		std::vector<Client*>	get_clients() const;
		std::vector<Client*>	get_partedClients() const;
		std::vector<Client*>	get_operators() const;
		Client*					get_owner() const;
		size_t	                get_userlimit() const;

        //Setters
		int		set_invite(Client* client);
		int		set_modes(uint8_t newmodes);
		int		set_password(std::string password, Client* client);
		int		set_limit(size_t limit, Client* client);
		int		set_topic_operator(Client* client);
		int		set_topic(std::string topic);


        //Others (Channel commands)
		bool	add_client(Client* client);
		int		client_rejoin(Client *client);
		bool	part_client(Client* client);
		bool	remove_client(Client* client);
		bool	remove_parted_client(Client* client);
		int		remove_operator(Client* client);
		int		client_in_channel(Client* client);
		int		client_was_in_channel(Client* client);
		int		client_is_operator(Client* client);

		//Others (User actions)
		int		leave_channel(Client* client);
		int		kick(Client* to_kick, Client* kicker);
		int		add_operator(Client* to_promote, Client* promoter);
		int		remove_operator(Client* to_demote, Client* demoter);
		int		invite(Client* invitee, Client* inviter);
		int		remove_invite(Client* invitee, Client* inviter);
		bool	is_invited(std::string nickname);

		void	msg_to_channel(Client *client, std::string msg);
};

#endif