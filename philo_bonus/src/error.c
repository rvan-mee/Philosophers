/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:41:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/18 20:08:02 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <stdlib.h>

void	exit_with_error(char *error, t_info *info, int32_t exit_code)
{
	if (error)
		printf("%s", error);
	if (info->forks != SEM_FAILED)
	{
		sem_close(info->forks);
		sem_unlink(SEM_FORKS);
	}
	if (info->eat_limit_sem != SEM_FAILED)
	{
		sem_close(info->eat_limit_sem);
		sem_unlink(SEM_EAT_LIM);
	}
	if (info->print != SEM_FAILED)
	{
		sem_close(info->print);
		sem_unlink(SEM_PRINT);
	}
	exit(exit_code);
}
