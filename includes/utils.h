/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 17:26:28 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/31 16:20:11 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <vector>
# include <string>

std::vector<std::string>	split(std::string const & message, char const delimiter);
std::vector<std::string> 	split(std::string const & message, std::string const delimiter);
bool	is_whitespace(char c);
void	trim_whitespace(std::string & message);


#endif
