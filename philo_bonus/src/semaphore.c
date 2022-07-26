/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   semaphore.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/16 17:24:21 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:08:27 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <errno.h>

/**
 * @brief		Function to create and initialize the fork,
 * 				eat limit and print semaphores.
 * @param info	Pointer to the info struct.
 * @return		N/A / Exit if sem_open fails 
*/
void	create_semaphores(t_info *info)
{
	sem_unlink(SEM_FORKS);
	info->forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, \
							0660, info->philos_count);
	if (info->forks == SEM_FAILED)
		exit_with_error(SEM_FAIL, ERROR);
	sem_unlink(SEM_FORKS);
	if (info->eat_limit_on)
	{
		sem_unlink(SEM_EAT_LIM);
		info->eat_limit_sem = sem_open(SEM_EAT_LIM, O_CREAT | O_EXCL, 0660, 0);
		if (info->eat_limit_sem == SEM_FAILED)
			exit_with_error(SEM_FAIL, ERROR);
		sem_unlink(SEM_EAT_LIM);
	}
	sem_unlink(SEM_PRINT);
	info->print = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0660, 1);
	if (info->print == SEM_FAILED)
		exit_with_error(SEM_FAIL, ERROR);
	sem_unlink(SEM_PRINT);
}
