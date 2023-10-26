#include <iostream>
#include <fstream>
#include <unordered_map>
#include "config.hpp"

Config::Config()
{
	_host = "default";
	_serverName = "default";
	_userModes = "";
	_channelModes = "";
	_maxClients = 4096;
}

Config::~Config()
{
}

std::string	Config::get_host() const
{
	return (_host);
}

std::string	Config::get_serverName() const
{
	return (_serverName);
}

std::string	Config::get_userModes() const
{
	return (_userModes);
}

std::string	Config::get_channelModes() const
{
	return (_channelModes);
}

size_t		Config::get_maxClients() const
{
	return (_maxClients);
}

void		Config::print_config() const
{
	std::cout << "host: " << _host << std::endl;
	std::cout << "serverName: " << _serverName << std::endl;
	std::cout << "userModes: " << _userModes << std::endl;
	std::cout << "channelModes: " << _channelModes << std::endl;
	std::cout << "maxClients: " << _maxClients << std::endl;
}