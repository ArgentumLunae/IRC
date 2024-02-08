#include <iostream>
#include <fstream>
#include <unordered_map>
#include "config.hpp"
#include <fstream>

#include "utils.h"

void	Config::config_from_file(std::string filePath)
{
	std::ifstream	fileStream;
	char 			line[256];
	std::vector<std::string>	currentLine;

	fileStream.open(filePath);
	if (!fileStream.is_open())
		return ;
	fileStream.getline(line, 256);
	currentLine = split(line, " ");
	while (!currentLine[0].empty())
	{
		if (currentLine.size() < 3)
			continue ;
		else if (currentLine[0] == "HOST")
			_host = currentLine[2];
		else if (currentLine[0] == "NAME")
			_serverName = currentLine[2];
		else if (currentLine[0] == "UMODES")
			_userModes = currentLine[2];
		else if (currentLine[0] == "CMODES")
			_channelModes = currentLine[2];
		else if (currentLine[0] == "MAX_CLIENTS")
			_maxClients = std::stoi(currentLine[2]);
		fileStream.getline(line, 256);
		currentLine = split(line, " ");
	}
	fileStream.close();
}

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