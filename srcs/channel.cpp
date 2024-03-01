#include "server.hpp"
#include "process_message.hpp"

#include <algorithm>

//CONSTRUCTORS AND DECONSTRUCTOS

Channel::Channel(std::string name, Client* creator, Server* server) : _server(server), _owner(creator)
{
	_name = name;
	_topic = "";
	_password = "";
	_invite = false;
	_userlimit = server->get_config().get_maxClients();
	_owner = creator;
	_clients = std::vector<Client*>{creator};
	_operators = std::vector<Client*>{creator};

	std::cout << _clients.front()->get_nickname() << std::endl;
	std::cout << _operators.front()->get_nickname() << std::endl;
}

Channel::Channel(const Channel &copy) {
	*this = copy;
}

Channel::~Channel()
{
}

Channel &Channel::operator=(const Channel &copy)
{
	_name = copy._name;
	_topic = copy._topic;
	_password = copy._password;
	_invite = copy._invite;
	_userlimit = copy._userlimit;
	_clients = copy._clients;
	_operators = copy._operators;
	_server = copy._server;
	_owner = copy._owner;
	_invitelist = copy._invitelist;
	return(*this);
}

//GETTERS

std::string	Channel::get_name() const
{
	return (_name);
}

std::string	Channel::get_topic() const
{
	return (_topic);
}

std::string	Channel::get_password() const
{
	return (_password);
}

bool Channel::get_invite() const
{
	return (_invite);
}

size_t Channel::get_userlimit() const
{
	return (_userlimit);
}

Server* Channel::get_server() const
{
	return (_server);
}

std::vector<Client*>	Channel::get_clients() const
{
	return (_clients);
}

std::vector<Client*>	Channel::get_partedClients() const
{
	return (_partedClients);
}

std::vector<Client*>	Channel::get_operators() const
{
	return (_operators);
}

Client*	Channel::get_owner() const
{
	return (_owner);
}

///SETTERS

int	Channel::set_invite(Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (check_operator_priv(client) == false)
		return (FAILURE);
	_invite = !_invite;
	return (SUCCESS);
}

//do we need specific password settings/rules that we need to abide by?
int	Channel::set_password(std::string password, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (check_operator_priv(client) == false)
		return (FAILURE);
	_password = password;
	return (SUCCESS);
}

int Channel::set_limit(size_t limit, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (check_operator_priv(client) == false)
		return (FAILURE);
	_userlimit = limit;
	return (SUCCESS);
}

int	Channel::set_topic(std::string topic)
{
	_topic = topic;
	return (SUCCESS);
}

// MEMBER FUNCTIONS

bool	Channel::add_client(Client* client)
{
	if (client == nullptr)
		return FAILURE;
	if (_clients.empty())
	{
		_clients.push_back(client);
		client->add_channel(this);
		return SUCCESS;
	}
	for (size_t idx = 0; idx < _clients.size(); idx++)
	{
		if (_clients.at(idx) == client)
		{
			std::cout << "Client already in channel: " << _clients[idx]->get_nickname() << " at index " << idx << std::endl; // FOR TESTING, REMOVE LATER.
			return SUCCESS;
		}
	}
	_clients.push_back(client);
	client->add_channel(this);
	if (_operators.empty())
		_operators.push_back(client);
	else
		std::cout << "FIRST OPERATOR: " << _operators[0]->get_nickname() << std::endl;
	msg_to_channel(client, ":" + client->get_nickname() + "!" + client->get_username() + "@" + _server->get_config().get_host() + " JOIN :" + _name);
	return SUCCESS;
}

int	Channel::client_rejoin(Client* client)
{
	if (client == nullptr)
		return FAILURE;
	for (std::vector<Client*>::iterator iter = _partedClients.begin(); iter < _partedClients.end(); iter++)
	{
		if (*iter != client)
			continue ;
		join_command(client, std::vector<std::string>{"part", _name, _password}, _server);
		_partedClients.erase(iter);
		std::cout << "Client #" << client->get_fd() << " has rejoined " << _name << std::endl;
		return SUCCESS;
	}
	return FAILURE;
}

bool	Channel::remove_client(Client* client)
{
	int clientPos;

	if(client == nullptr)
		return FAILURE;
	clientPos = client_in_channel(client);
	if (clientPos >= 0)
	{
		_clients.erase(_clients.begin() + clientPos);
		_partedClients.push_back(client);
	}
	return SUCCESS;
}

int	Channel::remove_operator(Client* client)
{
	int clientPos;

	if(client == nullptr)
		return FAILURE;
	clientPos = client_is_operator(client);
	if (clientPos >= 0)
		_operators.erase(_operators.begin() + clientPos);
	if (_operators.empty() && !_clients.empty())
	{
		_operators.push_back(_clients.front()); //employ make operator command
		_server->msg_to_client(_clients.front()->get_fd(), ":" + _server->get_config().get_host() + " 381 " + _name + ":You're now a channel operator.");
	}
	return SUCCESS;
}


int	Channel::client_in_channel(Client* client)
{
	for (size_t idx= 0; idx < _clients.size(); idx++)
	{
		if (_clients[idx] == client)
			return idx;
	}
	return -1;
}

int	Channel::client_was_in_channel(Client* client)
{
	for (size_t idx= 0; idx < _partedClients.size(); idx++)
	{
		if (_partedClients[idx] == client)
			return idx;
	}
	return -1;
}

int	Channel::client_is_operator(Client* client)
{
	for (size_t idx= 0; idx < _operators.size(); idx++)
	{
		if (_operators[idx] == client)
			return idx;
	}
	return -1;
}

bool Channel::check_operator_priv(Client *client)
{
	std::vector<Client *>::iterator itend = _operators.end();
	for (std::vector<Client *>::iterator it = _operators.begin(); it != itend; it++)
	{
		if (it[0] == client)
			return true;
	}
	return false;
}

void	Channel::msg_to_channel(Client *client, std::string msg)
{
	for (std::vector<Client*>::iterator iter = _clients.begin(); iter != _clients.end(); iter++)
	{
		if (*iter == client)
			continue ;
		_server->msg_to_client((*iter)->get_fd(), msg);
	}
}