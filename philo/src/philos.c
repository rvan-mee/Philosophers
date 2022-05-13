/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philos.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:44:23 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/13 14:51:41 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
	* Function to initiate a given philosopher struct 
	* with the given parameters.
	* @param *info pointer to the info struct that 
	* will be stored inside the philosopher.
	* @param *philos a pointer to the philosopher 
	* where the data has to be set.
	* @param i intiger resembling the philosopher's id.
*/
void	set_philo_values(t_info *info, t_philosopher *philos, int i)
{
	philos[i].info = info;
	philos[i].id = i;
	philos[i].left = i;
	philos[i].right = (i + 1) % info->philos_count;
	philos[i].meals_eaten = 0;
	philos[i].last_meal_time = get_current_time_ms();
}

/*
	* Funciton to handle message printing.
	* Before a message is printed it checks if a philosoper
	* has died or (if specified) the eat limit is reached.
	* @param *philo The philosopher that is sending the message.
	* @param msg An integer to choose what message
	* to print (defined in the header).
	* @return if the philosopher has died whilst trying to print
	* a message [false] else [true].
*/
bool	display_msg_if_not_dead(t_philosopher *philo, int msg)
{
	int	time;

	time = time_since_start(philo);
	if (check_for_death_and_eat_limit(philo) == true)
		return (false);
	if (msg == TAKE_FORK)
		printf("%d %d has taken a fork\n", time, philo->id + 1);
	if (msg == EATING)
		printf("%d %d is eating\n", time, philo->id + 1);
	if (msg == SLEEPING)
		printf("%d %d is sleeping\n", time, philo->id + 1);
	if (msg == THINKING)
		printf("%d %d is thinking\n", time, philo->id + 1);
	if (msg == DIED)
		printf("%d %d died\n", time, philo->id + 1);
	return (true);
}

/*
	* Fuction to handle a single philosopher.
	* @param *philo Pointer to the philosoper.
*/
static void	*single_philo(t_philosopher *philo)
{
	display_msg_if_not_dead(philo, TAKE_FORK);
	pthread_mutex_lock(&philo->info->fork_mutex[philo->right]);
	wait_set_time(philo->info->time_die, philo);
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
	return (NULL);
}

/*
	* Function to check if all threads were created.
	* @param *philo Pointer to the philosopher to access thread data.
	* @return If all threads were created properly [true] else [false].
*/
static bool	thread_creation_check(t_philosopher *philo)
{
	int	created_threads;

	pthread_mutex_lock(&philo->info->creation_check);
	created_threads = philo->info->created_threads;
	pthread_mutex_unlock(&philo->info->creation_check);
	if (created_threads != philo->info->philos_count)
		return (false);
	return (true);
}

/*
	* Main philosopher function that calls all the tasks.
	* @param *threadstruct Void pointer that is being cast to
	* a philosopher struct.
*/
void	*philosopher(void *threadstruct)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)threadstruct;
	if (thread_creation_check(philo) == false)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (philo->info->philos_count == 1)
		return (single_philo(philo));
	if (philo->id % 2 == 1)
		usleep(500);
	while (1)
	{
		if (!take_forks(philo))
			break ;
		if (!start_eating(philo))
			break ;
		if (!start_sleeping(philo))
			break ;
		if (!start_thinking(philo))
			break ;
	}
	return (NULL);
}
