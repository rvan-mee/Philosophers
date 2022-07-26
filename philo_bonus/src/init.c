/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/18 12:02:06 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 14:38:19 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief			Function to set the named semaphores name inside the
 * 					philosopher_data struct.
 * @param	philo	Pointer to the philosopher_data struct
 * 					which needs its semaphore name set.
 * @param	id		The id of the pilosopher.
 * @return			N/A
*/
static void	set_checking_sem_name(t_philo_data *philo, int32_t id)
{
	size_t	i;

	i = 0;
	while (i < 7)
	{
		philo->checking_sem_name[i] = "/philo_"[i];
		i++;
	}
	if (id == 0)
		philo->checking_sem_name[i++] = (id % 10 + '0');
	while (id != 0)
	{
		philo->checking_sem_name[i] = (id % 10 + '0');
		id /= 10;
		i++;
	}
	philo->checking_sem_name[i] = '\0';
}

/**
 * @brief		Function used to initialize the checking semaphore for a philo.
 * @param philo	Pointer to a philosopher's struct containing the semaphore.
 * @return		0 Success / -1 sem_open Failed
*/
static int32_t	create_checking_semaphore(t_philo_data *philo)
{
	sem_unlink(philo->checking_sem_name);
	philo->checking_sem = sem_open(philo->checking_sem_name, \
									O_CREAT | O_EXCL, 0660, 1);
	if (philo->checking_sem == SEM_FAILED)
	{
		printf("%s", SEM_FAIL);
		return (-1);
	}
	sem_unlink(philo->checking_sem_name);
	return (0);
}

/**
 * @brief		Function to initialize all philos and their data.
 * @param info	Pointer to the info struct.
 * @return		N/A / Exit on any fails
*/
void	init_philo_data(t_info *info)
{
	size_t	i;

	i = 0;
	info->print = SEM_FAILED;
	info->forks = SEM_FAILED;
	info->eat_limit_sem = SEM_FAILED;
	info->data = malloc(sizeof(t_philo_data) * info->philos_count);
	if (!info->data)
		exit_with_error(MALLOC_FAIL, ERROR);
	while (i != info->philos_count)
	{
		info->data[i].meals_eaten = 0;
		info->data[i].has_posted = false;
		info->data[i].last_meal_time = get_current_time_ms();
		info->data[i].time_till_start = get_current_time_ms() + 200;
		set_checking_sem_name(&info->data[i], i);
		if (create_checking_semaphore(&info->data[i]) == -1)
		{
			free(info->data);
			exit_with_error(NULL, ERROR);
		}
		i++;
	}
}
