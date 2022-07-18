/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:34:06 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/18 15:44:07 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <signal.h>
#include <stdio.h>

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
	exit_with_error(NULL, info, exit_code);
}
