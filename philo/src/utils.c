/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:58:37 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/10 12:50:17 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void	set_philos_dead(t_info *info)
{
	pthread_mutex_lock(&info->death_check_mutex);
	info->philo_has_died = true;
	pthread_mutex_unlock(&info->death_check_mutex);
}

void	monitor_philos(t_info *info, t_philosopher *philo)
{
	int		i;
	int		philos_with_max_meals;

	while (1)
	{
		i = 0;
		philos_with_max_meals = 0;
		while (i < philo->info->philos_count)
		{
			if (info->eat_limit_on == true)
			{
				pthread_mutex_lock(&philo[i].eat_mutex);
				if (philo[i].meals_eaten >= info->min_times_to_eat)
					philos_with_max_meals++;
				pthread_mutex_unlock(&philo[i].eat_mutex);
			}
			if (info->eat_limit_on == true
				&& philo->info->philos_count == philos_with_max_meals)
				return (set_philos_dead(info));
			if (check_if_should_die(&philo[i]) == true)
				return ;
			i++;
		}
		usleep(250);
	}
}

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
	pthread_mutex_lock(&philo->info->death_check_mutex);
	if (philo->info->philo_has_died == true)
	{
		pthread_mutex_unlock(&philo->info->death_check_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->death_check_mutex);
	return (false);
}

bool	check_for_death_and_eat_limit(t_philosopher *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philo->info->death_check_mutex);
	if (philo->info->philo_has_died == true)
	{
		pthread_mutex_unlock(&philo->info->death_check_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->death_check_mutex);
	return (false);
}
