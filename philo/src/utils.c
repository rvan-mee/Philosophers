/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:58:37 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/13 14:22:23 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/*
	* Function to set the philo_has_died boolian inside the info struct to true.
	* @param *info pointer to the info struct containing philo_has_died.
*/
void	set_philos_dead(t_info *info)
{
	pthread_mutex_lock(&info->death_check_mutex);
	info->philo_has_died = true;
	pthread_mutex_unlock(&info->death_check_mutex);
}

/*
	* Function to monitor all the philosophers for their deaths
	* and (if specified) if the eat limit has been reached. 
	* @param *info Pointer to the info struct containing the eat limit information.
	* @param *philo Pointer to an array of philosophers to monitor.
*/
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

/*
	* Function to check if the death or eat limit has been reached.
	* @param *philo pointer to a philosopher to get access to the death data.
	* @return if a philosopher has died
	* or reached the eat limit [true] else [false].
*/
bool	check_for_death_and_eat_limit(t_philosopher *philo)
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

/*
	* Function to unlock the given philosopher's fork mutexes.
	* @param *philo Pointer to the philosopher who's mutexes need
	* to be unlocked.
*/
void	unlock_both_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->left]);
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
}
