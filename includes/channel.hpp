#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include "client.hpp"

class Channel
{
	private:
		std::string	_name;
		std::string	_topic;
		std::string	_password;
		bool		_invite;
		Server*		_server;
		std::vector<Client*>	_clients;
		std::vector<Client*>	_operators;
		std::vector<Client*>	_invitelist;
		size_t		_userlimit;

	public:
        //Constructors/Deconstructors
		Channel(std::string name, Client* creator, Server *server);
		Channel(const Channel &copy);
		~Channel();
		Channel	&operator=(const Channel &copy);

		//Getters
		std::string				get_name() const;
		std::string				get_topic() const;
		bool					get_topic_operator() const;
		std::string				get_password() const;
		bool					get_invite() const;
		Server*					get_server() const;
		std::vector<Client*>	get_clients() const;
		std::vector<Client*>	get_operators() const;
		Client*					get_owner() const;
		size_t	                get_userlimit() const;

        //Setters
		int		set_invite(Client* client);
		int		set_password(std::string password, Client* client);
		int		set_limit(size_t limit, Client* client);
		int		set_topic_operator(Client* client);
		int		set_topic(std::string topic, Client* client);


        //Others (Channel commands)
		bool	add_client(Client* client);
		bool	remove_client(Client* client);
		bool	client_in_channel(std::string nickname) const;
		bool	client_is_operator(std::string nickname) const;

		//Others (User actions)
		int		leave_channel(Client* client);
		int		kick(Client* to_kick, Client* kicker);
		int		add_operator(Client* to_promote, Client* promoter);
		int		remove_operator(Client* to_demote, Client* demoter);
		int		invite(Client* invitee, Client* inviter);
		int		remove_invite(Client* invitee, Client* inviter);
		bool	is_invited(std::string nickname);
};

#endif