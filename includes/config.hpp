#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>

class Config
{
	private:
		std::string	_host;
		std::string	_serverName;
		std::string	_userModes;
		std::string	_channelModes;
		size_t		_maxClients;
        
	public:
        //Constructors/Deconstructors
		Config();
		~Config();

        //Getters
		std::string	get_host() const;
		std::string	get_serverName() const;
		std::string	get_userModes() const;
		std::string	get_channelModes() const;
		size_t		get_maxClients() const;

        //Setters

        //Others
		void		print_config() const;
};

#endif