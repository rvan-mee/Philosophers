/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   forks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:52:42 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/11 11:57:29 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

bool	init_philos(t_info *info, t_philosopher *philos)
{
	int				i;

	i = 0;
	info->fork_mutex = malloc(sizeof(pthread_mutex_t) * info->philos_count);
	if (!info->fork_mutex)
		return (false);
	while (i < info->philos_count)
	{
		if (set_philo_values(info, philos, i) == false)
			return (false);
		if (pthread_mutex_init(&info->fork_mutex[i], NULL) != 0)
			return (false);
		if (pthread_mutex_init(&philos[i].meal_time_mutex, NULL) != 0)
			return (false);
		if (philos->info->eat_limit_on == true)
			if (pthread_mutex_init(&philos[i].eat_mutex, NULL) != 0)
				return (false);
		i++;
	}
	return (true);
}

int	destroy_forks(t_info *info, t_philosopher *philos)
{
	int	i;

	i = 0;
	if (info->philos_count == 1)
		return (0);
	while (i < info->philos_count)
	{
		pthread_mutex_destroy(&info->fork_mutex[i]);
		if (info->eat_limit_on == true)
			pthread_mutex_destroy(&philos[i].eat_mutex);
		i++;
	}
	return (0);
}

void	unlock_both_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->left]);
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
}
