#include "client.hpp"
#include "server.hpp"

//CONSTRUCTORS/DECONSTRUCTORS

Client::Client(int fd, Server* server) : _server(server)
{
    _fd = fd;
	_nickname = "";
	_username = "";
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

int	Client::set_nickname(std::string nickname)
{
	//!!!!ADD CHECKS FOR IF NICKNAME IS ACTUALLY VALID BEFORE ANY OF THIS OTHER STUFF!!!!
	
	_nickname = nickname;
	//SET THE FULLNAME HERE
	return (SUCCESS);
}

int	Client::set_username(std::string username)
{
    //!!!!ADD CHECKS FOR IF USERNAME IS ACTUALLY VALID BEFORE ANY OF THIS OTHER STUFF!!!!

	_username = username;
	//SET THE FULLNAME HERE
	return (true);
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

//OTHERS (STILL TO DO)

int			Client::join_channel(std::string channelName, std::string password)
{
	channelName = password;
	return 1;
}

int			Client::leave_channel(std::string channelName)
{
	channelName = "me";
	return 1;
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

void	Client::finish_registration(void)
{
	std::cout << "finish_registration()" << std::endl;
	if (_correctPassword == false || _capabilityNegotiation \
		|| _nickname.empty() || _username.empty())
	{
		std::cout << "correctPassword: " << _correctPassword << std::endl;
		return ;
	}
	_registered = true;
	_server->msg_to_client(_fd, ":127.0.0.1 001 server :Welcome to the server\r\n");
}
