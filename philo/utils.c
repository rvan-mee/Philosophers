/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:58:37 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/06 15:39:45 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

bool	check_for_death(t_philosopher *philo)
{
	pthread_mutex_lock(philo->info->death_check_mutex);
	if (philo->info->philo_has_died == true)
	{
		pthread_mutex_unlock(philo->info->death_check_mutex);
		return (true);
	}
	pthread_mutex_unlock(philo->info->death_check_mutex);
	return (false);
}

bool	check_for_death_and_eat_limit(t_philosopher *philo)
{
	int	i;

	i = 0;
	if (philo->info->eat_limit_on == true)
	{
		while (i < philo->info->philos_count)
		{
			pthread_mutex_lock(philo->info->eat_mutex);
			if (philo->info->meals_eaten[i] < philo->info->min_times_to_eat)
			{
				pthread_mutex_unlock(philo->info->eat_mutex);
				break ;
			}
			pthread_mutex_unlock(philo->info->eat_mutex);
			i++;
		}
	}
	if (check_for_death(philo) == true || i == philo->info->philos_count)
		return (true);
	return (false);
}
