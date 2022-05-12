/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tasks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 15:30:25 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/12 14:38:56 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>
#include <unistd.h>

/*
	* Function that attempts to take both the 'left' and 'right'
	* forks next to a philosopher.
	* Calls display_msg_if_not_dead to print the fork message.
	* @param *philo Pointer to the philosopher that should try to take
	* both the forks.
	* @return if the philosopher has died whilst trying to print
	* a message [false] else [true].
*/
bool	take_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->info->fork_mutex[philo->right]);
	if (display_msg_if_not_dead(philo, TAKE_FORK) == false)
	{
		pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
		return (false);
	}
	pthread_mutex_lock(&philo->info->fork_mutex[philo->left]);
	if (display_msg_if_not_dead(philo, TAKE_FORK) == false)
	{
		pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
		pthread_mutex_unlock(&philo->info->fork_mutex[philo->left]);
		return (false);
	}
	return (true);
}

/*
	* Function that resets the [last_meal_time] to the current time and
	* (if eat limit is specified) ups the amount of meals eaten by 1.
	* after eating it will wait [time_eat] and release both the forks
	* the given philosopher is holding.
	* @param *philo Pointer to the philosopher that should start eating.
	* @return if the philosopher has died whilst trying to print
	* a message [false] else [true].
*/
bool	start_eating(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (display_msg_if_not_dead(philo, EATING) == false)
	{
		unlock_both_forks(philo);
		return (false);
	}
	if (philo->info->eat_limit_on == true)
	{
		pthread_mutex_lock(&philo->eat_mutex);
		philo->meals_eaten += 1;
		pthread_mutex_unlock(&philo->eat_mutex);
	}
	wait_set_time(philo->info->time_eat, philo);
	unlock_both_forks(philo);
	return (true);
}

/*
	* Function that calls display_msg_if_not_dead with the sleeping argument.
	* After displaying a message it will wait [time_to_sleep]
	* amount of miliseconds.
	* @param *philo the philosopher that should be sleeping.
	* @return if the philosopher has died whilst trying to print
	* a message [false] else [true].
*/
bool	start_sleeping(t_philosopher *philo)
{
	if (display_msg_if_not_dead(philo, SLEEPING) == false)
		return (false);
	wait_set_time(philo->info->time_sleep, philo);
	return (true);
}

/*
	* Function that calls display_msg_if_not_dead with the thinking argument.
	* @param *philo pointer to the philo that should be thinking.
	* @return if the philosopher has died whilst trying to print
	* a message [false] else [true].
*/
bool	start_thinking(t_philosopher *philo)
{
	if (display_msg_if_not_dead(philo, THINKING) == false)
		return (false);
	return (true);
}

/*
	* Function that checks and sets the given philosopher to dead.
	* calls display_msg_if_not_dead with the death message.
	* @param *philo pointer to the philosopher to check.
	* @return If the philosopher died [true] else [false]
*/
bool	check_if_should_die(t_philosopher *philo)
{
	long	current_time;

	pthread_mutex_lock(&philo->meal_time_mutex);
	current_time = get_current_time_ms();
	if (current_time - philo->last_meal_time > philo->info->time_die)
	{
		pthread_mutex_unlock(&philo->meal_time_mutex);
		pthread_mutex_lock(&philo->info->death_check_mutex);
		if (philo->info->philo_has_died != true)
		{
			pthread_mutex_unlock(&philo->info->death_check_mutex);
			display_msg_if_not_dead(philo, DIED);
			pthread_mutex_lock(&philo->info->death_check_mutex);
			philo->info->philo_has_died = true;
			pthread_mutex_unlock(&philo->info->death_check_mutex);
		}
		else
			pthread_mutex_unlock(&philo->info->death_check_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->meal_time_mutex);
	return (false);
}
