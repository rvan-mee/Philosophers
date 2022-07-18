/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   semaphore.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/16 17:24:21 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/18 20:02:41 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <errno.h>

void	create_semaphores(t_info *info)
{
	sem_unlink(SEM_FORKS);
	info->forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, \
							0660, info->philos_count);
	if (info->forks == SEM_FAILED)
		exit_with_error(SEM_FAIL, info, ERROR);
	if (info->eat_limit_on)
	{
		sem_unlink(SEM_EAT_LIM);
		info->eat_limit_sem = sem_open(SEM_EAT_LIM, O_CREAT | O_EXCL, 0660, 1);
		if (info->eat_limit_sem == SEM_FAILED)
			exit_with_error(SEM_FAIL, info, ERROR);
	}
	sem_unlink(SEM_PRINT);
	info->print = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0660, 1);
	if (info->print == SEM_FAILED)
		exit_with_error(SEM_FAIL, info, ERROR);
}
