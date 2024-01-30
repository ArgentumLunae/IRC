/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   splitMsg.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mteerlin <mteerlin@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 14:41:29 by mteerlin      #+#    #+#                 */
/*   Updated: 2024/01/27 14:44:15 by mteerlin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int main(void)
{
    char * str = "he\0llo thi\0s is a pa\0rtial message";

    write(1, str, 34);
    return (0);
}
