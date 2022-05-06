/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   forks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:52:42 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/06 12:44:25 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static void	connect_first_last_forks(t_philosopher *philos, int last)
{
	if (last != 0)
		philos[0].left_fork = philos[last].right_fork;
	else
		philos[0].left_fork = NULL;
}

pthread_mutex_t	*get_new_mutex(void)
{
	pthread_mutex_t	*new_mutex;

	new_mutex = malloc(sizeof(pthread_mutex_t) * 1);
	if (!new_mutex)
		return (NULL);
	if (pthread_mutex_init(new_mutex, NULL) != 0)
	{
		free(new_mutex);
		return (NULL);
	}
	return (new_mutex);
}

bool	init_philos(t_info *info, t_philosopher *philos)
{
	pthread_mutex_t	*new_fork;
	int				i;

	i = 0;
	while (i < info->philos_count)
	{
		philos[i].info = info;
		philos[i].id = i;
		new_fork = get_new_mutex();
		if (!new_fork)
			return (false);
		philos[i].right_fork = new_fork;
		if (i != 0)
			philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	connect_first_last_forks(philos, i - 1);
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
		pthread_mutex_destroy(philos[i].right_fork);
		free(philos[i].right_fork);
		i++;
	}
	if (info->eat_limit_on == false)
		return (0);
	pthread_mutex_destroy(info->eat_mutex);
	free(info->eat_mutex);
	free(info->meals_eaten);
	return (0);
}

void	unlock_both_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
