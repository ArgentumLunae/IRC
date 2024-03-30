#include "client.hpp"
#include "server.hpp"

//CONSTRUCTORS/DECONSTRUCTORS

Client::Client(int fd, Server* server) : _server(server)
{
    _fd = fd;
	_nickname = "";
	_username = "";
	_hostname = "";
	_hostmask = "";
	_correctPassword = false;
	_capabilityNegotiation = false;
	_registered = false;
	_channelList = std::vector<Channel*>();
	_messageBuffer = "";
}

Client::Client(const Client &copy) : _server(copy._server)
{
	*this = copy;
}

Client::~Client()
{
	std::cout << "Client #" << _fd << " has been shot, sir." << std::endl;
	return ;
}

Client	&Client::operator=(const Client &copy)
{
	_fd = copy._fd;
	_nickname = copy._nickname;
	_username = copy._username;
	_correctPassword = copy._correctPassword;
	_capabilityNegotiation = copy._capabilityNegotiation;
	_registered = copy._registered;
	_server = copy._server;
	_channelList = copy._channelList;
	return (*this);
}

//GETTERS

int	Client::get_fd() const {
	return (_fd);
}

std::string	Client::get_fullname() const {
	return (_fullname);
}

std::string	Client::get_nickname() const {
	return (_nickname);
}

std::string	Client::get_username() const {
	return (_username);
}

std::string Client::get_hostname() const {
	return _hostname;
}

std::string Client::get_hostmask() const {
	return _hostmask;
}

bool		Client::get_correctPassword() const {
	return (_correctPassword);
}

Server*		Client::get_server() const {
	return (_server);
}

std::vector<Channel*> Client::get_channelList(void) const {
	return (_channelList);
}

Channel*	Client::get_channel(std::string channelName)
{
	std::vector<Channel*>::iterator itend = _channelList.end();
	for (std::vector<Channel*>::iterator iter = _channelList.begin(); iter != itend; iter++)
	{
		if ((*iter)->get_name() == channelName)
			return *iter;
	}
	return nullptr;
}

std::string	Client::get_messageBuffer(void) const {
	return (_messageBuffer);
}

bool	Client::get_capabilityNegotiation(void) const {
	return (_capabilityNegotiation);
}

//SETTERS

int	Client::set_nickname(std::string const nickname)
{
	_nickname = nickname;
	return (SUCCESS);
}

int	Client::set_username(std::string const username)
{
	_username = username;
	_hostmask = _username + "@" + _hostname;
	return (true);
}

void Client::set_hostname(std::string const hostname) {
	_hostname = hostname;
	_hostmask = _username + "@" + _hostname;
}

int	Client::set_correctPassword(std::string password)
{
	if (password.empty() || password != get_server()->get_pass())
	{
		_correctPassword = false;
		return (false);
	}
	_correctPassword = true;
	return (true);
}

void	Client::set_capabilityNegotiation(bool state) {
	_capabilityNegotiation = state;
}

void	Client::set_registered(bool state) {
	_registered = state;
}

//OTHERS (STILL TO DO)

int			Client::add_channel(Channel *channel)
{
	if (!is_in_channel(channel->get_name()))
	{
		_channelList.push_back(channel);
		std::cout << _channelList.front()->get_name() << std::endl;
		return SUCCESS;
	}
	return FAILURE;
}

int			Client::join_channel(std::string channelName, std::string password)
{
	channelName = password;
	return 1;
}

int			Client::leave_channel(Channel *channel)
{
	for (std::vector<Channel*>::iterator iter = _channelList.begin(); iter != _channelList.end(); iter++)
	{
		if (*iter == channel)
		{
			channel->part_client(this);
			_partedChannels.push_back(*iter);
			_channelList.erase(iter);
			if ((*iter)->get_clients().empty()  && (*iter)->get_partedClients().empty())
				_server->remove_channel((*iter)->get_name());
			return SUCCESS;
		}
	}
	return FAILURE;
}

void			Client::leave_all_channels(void)
{
	for (std::vector<Channel*>::iterator iter = _channelList.begin(); iter != _channelList.end(); iter = _channelList.begin())
	{
			(*iter)->remove_client(this);
			(*iter)->remove_operator(this);
			(*iter)->remove_invite(this);
			_channelList.erase(iter);
			if ((*iter)->get_clients().empty()  && (*iter)->get_partedClients().empty())
				_server->remove_channel((*iter)->get_name());
			if (_channelList.empty())
				break ;
	}
	for (std::vector<Channel*>::iterator iter = _partedChannels.begin(); iter != _partedChannels.end(); iter = _partedChannels.begin())
	{
		std::cout << (*iter)->get_name() << std::endl;
		(*iter)->remove_parted_client(this);
		_partedChannels.erase(iter);
		if ((*iter)->get_clients().empty()  && (*iter)->get_partedClients().empty())
			_server->remove_channel((*iter)->get_name());
	}
}

bool		Client::is_in_channel(std::string channelName)
{
	for (std::vector<Channel*>::iterator iter = _channelList.begin(); iter != _channelList.end(); iter++)
	{
		if ((*iter)->get_name() == channelName)
			return true;
	}
	return false;
}

void	Client::push_message(std::string message)
{
	_incomingMessages.push_back(message);
}

std::string	Client::pop_message(void)
{
	std::string firstMessage = _incomingMessages.front();
	_incomingMessages.pop_front();
	return firstMessage;
}

bool	Client::has_incoming_messages(void) const
{
	return (!_incomingMessages.empty());
}

void	Client::add_to_message_buffer(std::string partialMessage)
{
	if (_messageBuffer.empty())
		_messageBuffer = partialMessage;
	else
		_messageBuffer += partialMessage;
}

void	Client::clear_message_buffer(void) {
		_messageBuffer.clear();
}

bool	Client::is_registered(void) const {
	return (_registered);
}
