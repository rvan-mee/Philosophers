/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tasks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 15:30:25 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/07 18:20:28 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>

bool	take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		if (take_right_fork(philo) == false)
			return (false);
		if (take_left_fork(philo) == false)
		{
			pthread_mutex_unlock(philo->right_fork);
			return (false);
		}
	}
	else
	{
		if (take_left_fork(philo) == false)
			return (false);
		if (take_right_fork(philo) == false)
		{
			pthread_mutex_unlock(philo->left_fork);
			return (false);
		}
	}
	return (true);
}

bool	start_eating(t_philosopher *philo)
{
	if (check_for_death_and_eat_limit(philo))
		return (false);
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
	philo_msg(philo, EATING);
	if (philo->info->eat_limit_on == true)
	{
		pthread_mutex_lock(philo->info->eat_mutex);
		philo->info->meals_eaten[philo->id] += 1;
		pthread_mutex_unlock(philo->info->eat_mutex);
	}
	wait_set_time(philo->info->time_eat, philo);
	unlock_both_forks(philo);
	return (true);
}

bool	start_sleeping(t_philosopher *philo)
{
	if (check_for_death_and_eat_limit(philo))
		return (false);
	philo_msg(philo, SLEEPING);
	wait_set_time(philo->info->time_eat, philo);
	return (true);
}

bool	start_thinking(t_philosopher *philo)
{
	if (check_for_death_and_eat_limit(philo))
		return (false);
	philo_msg(philo, THINKING);
	return (true);
}

bool	check_if_should_die(t_philosopher *philo)
{
	long	current_time;

	pthread_mutex_lock(philo->meal_time_mutex);
	current_time = get_current_time_ms();
	if (current_time - philo->last_meal_time >= philo->info->time_die)
	{
		pthread_mutex_unlock(philo->meal_time_mutex);
		pthread_mutex_lock(philo->info->death_check_mutex);
		if (philo->info->philo_has_died != true)
			philo_msg(philo, DIED);
		philo->info->philo_has_died = true;
		pthread_mutex_unlock(philo->info->death_check_mutex);
		return (true);
	}
	pthread_mutex_unlock(philo->meal_time_mutex);
	return (false);
}
