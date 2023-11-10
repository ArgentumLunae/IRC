#include "channel.hpp"
#include "client.hpp"
#include "server.hpp"

//CONSTRUCTORS AND DECONSTRUCTOS

Channel::Channel(std::string name, Client* creator, Server* server) : _server(_server), _creator(creator)
{
	_name = name;
	_topic = "";
	_password = "";
	_invite = false;
	_userlimit = 4096;
	_clients.push_back(creator);
	_operators.push_back(creator);
}

Channel::Channel(const Channel &copy) : _server(copy._server), _creator(copy._creator)
{
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
	_creator = copy._creator;
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

std::vector<Client*>	Channel::get_operators() const
{
	return (_operators);
}

Client*	Channel::get_creator() const
{
	return (_creator);
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

int	Channel::set_topic(std::string topic, Client* client)
{
	if (client == nullptr)
		return (FAILURE);
	if (check_operator_priv(client) == false)
		return (FAILURE);
	_topic = topic;
	return (SUCCESS);
}