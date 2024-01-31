/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   untils.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/10 11:57:45 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/31 16:20:46 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <sstream>

#include <iostream>

std::vector<std::string> split(std::string const & message, char const delimiter)
{
	std::vector<std::string> split;
	std::stringstream	strStream(message);
	std::string			token;

	while (std::getline(strStream, token, delimiter))
		split.push_back(token);
	return split;
}

std::vector<std::string> split(std::string const & message, std::string const delimiter)
{
	std::vector<std::string> split;
	std::string token;
	size_t delimPos = 0;
	size_t tokenStart = 0;

	while((delimPos = message.find(delimiter, tokenStart)) != std::string::npos)
	{
		token = message.substr(tokenStart, delimPos - tokenStart);
		split.push_back(token);
		tokenStart = delimPos + delimiter.length();
	}
	split.push_back(message.substr(tokenStart));
	return split;
}

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return true;
	return false;
}

void	trim_whitespace(std::string & message)
{
	size_t	idx = 0;

	while(is_whitespace(message[idx]))
		idx++;
	message = message.substr(idx, message.length() - idx);
}
