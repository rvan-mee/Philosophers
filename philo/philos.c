/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philos.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:44:23 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/08 15:47:11 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

bool	set_philo_values(t_info *info, t_philosopher *philos, int i)
{
	pthread_mutex_t	*new_fork;
	pthread_mutex_t	*meal_mutex;

	philos[i].info = info;
	philos[i].id = i;
	new_fork = get_new_mutex();
	if (!new_fork)
		return (false);
	meal_mutex = get_new_mutex();
	if (!meal_mutex)
	{
		pthread_mutex_destroy(new_fork);
		return (false);
	}
	philos[i].right_fork = new_fork;
	philos[i].meal_time_mutex = meal_mutex;
	philos[i].last_meal_time = get_current_time_ms();
	if (i != 0)
		philos[i].left_fork = philos[i - 1].right_fork;
	return (true);
}

void	philo_msg(t_philosopher *philo, int msg)
{
	int	time;

	time = time_since_start(philo);
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
}

static void	*single_philo(t_philosopher *philo)
{
	philo_msg(philo, TAKE_FORK);
	pthread_mutex_lock(philo->right_fork);
	wait_set_time(philo->info->time_die, philo);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_destroy(philo->right_fork);
	if (philo->info->eat_limit_on == true)
	{
		pthread_mutex_destroy(philo->info->eat_mutex);
		free(philo->info->eat_mutex);
		free(philo->info->meals_eaten);
	}
	free(philo->right_fork);
	return (NULL);
}

void	*philosopher(void *threadstruct)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)threadstruct;
	if (philo->id % 2 == 1)
		usleep(50);
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
	if (philo->info->philos_count == 1)
		return (single_philo(philo));
	while (check_for_death_and_eat_limit(philo) == false)
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
