/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:41:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:18:09 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief		Function used to get the lenght of a string.
 * @param str	Pointer to the string to get the length of.
 * @return		Lentgh of the string.
*/
static size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

/**
 * @brief 			Function used to exit with a given error message
 * @param error 	Pointer to a string containing an error message.
 * 					Won't be printed if NULL.
 * @param exit_code	Integer used as the exit status of the proccess.
 * @return 			N/A
*/
void	exit_with_error(char *error, int32_t exit_code)
{
	if (error)
		write(STDERR_FILENO, error, ft_strlen(error));
	exit(exit_code);
}
