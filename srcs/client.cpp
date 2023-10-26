#include "client.hpp"
#include "server.hpp"
#include "channel.hpp"

//CONSTRUCTORS/DECONSTRUCTORS

Client::Client(int fd, Server* sever) : _server(_server)
{
    _fd = fd;
	_nickname = "";
	_username = "";
	_correctPassword = false;
	_channelList = std::vector<Channel*>();
}

Client::Client(const Client &copy) : _server(copy._server)
{
	*this = copy;
}

Client::~Client()
{
	std::cout << "Client's been shot, sir." << std::endl;
	return ;
}

Client	&Client::operator=(const Client &copy)
{
	_fd = copy._fd;
	_nickname = copy._nickname;
	_username = copy._username;
	_server = copy._server;
	_channelList = copy._channelList;
	return (*this);
}

//GETTERS

int	Client::get_fd() const
{
	return (_fd);
}

std::string	Client::get_fullname() const
{
	return (_fullname);
}

std::string	Client::get_nickname() const
{
	return (_nickname);
}

std::string	Client::get_username() const
{
	return (_username);
}

bool		Client::get_correctPassword() const
{
	return (_correctPassword);
}

Server*		Client::get_server() const
{
	return (_server);
}

std::vector<Channel*> Client::get_channelList(void)
{
	return (_channelList);
}

//SETTERS

int	Client::set_nickname(std::string nickname)
{
    //!!!!ADD CHECKS FOR IF NICKNAME IS ACTUALLY VALID BEFORE ANY OF THIS OTHER STUFF!!!!

	_nickname = nickname;
    //SET THE FULLNAME HERE
	return (true);
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
	if (password != get_server()->get_pass())
	{
		_correctPassword = false;
		return (false);
	}
	_correctPassword = true;
	return (true);
}

//OTHERS (STILL TO DO)

int			join_channel(std::string channelName, std::string password);
int			leave_channel(std::string channelName);
std::vector<Channel*> get_channelList(void);
Channel*	get_channel(std::string channelName);
bool		is_in_channel(std::string channelName);
