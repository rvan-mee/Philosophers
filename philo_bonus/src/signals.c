/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:34:06 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:13:25 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>
#include <stdio.h>

/**
 * @brief					Function used to kill all child proccesses and
 * 							exit with the given exit_code.
 * @param str				Pointer to a string that will be displayed before
 * 							killing all child proccessed. This can be NULL.
 * @param info				Pointer to the info struct.
 * @param created_philos	Number of proccesses created.
 * @param exit_code			Integer used to set the exit status.
 * @return					Exit
*/
void	execute_children(char *str, t_info *info, \
						uint32_t created_philos, int32_t exit_code)
{
	size_t	i;

	i = 0;
	if (str)
		printf("%s", str);
	while (i < created_philos)
	{
		kill(info->philos[i], SIGKILL);
		i++;
	}
	exit_with_error(NULL, exit_code);
}
