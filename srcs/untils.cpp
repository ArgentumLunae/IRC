/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   untils.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/10 11:57:45 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/24 17:21:42 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> split(std::string const message, char const delimiter)
{
	std::vector<std::string> split;
	std::stringstream	strStream(message);
	std::string			token;

	while (std::getline(strStream, token, delimiter))
		split.push_back(token);
	return split;
}

std::vector<std::string> split(std::string const message, char const * delimiter)
{
	std::vector<std::string> split;
	std::string token;
	size_t delimPos = 0;
	size_t tokenStart = 0;

	while((delimPos = message.find(delimiter)) != std::string::npos)
	{
		token = message.substr(tokenStart, delimPos - tokenStart);
		split.push_back(token);
		tokenStart = delimPos + 1;
	}
}
