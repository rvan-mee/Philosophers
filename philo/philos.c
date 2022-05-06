/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philos.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:44:23 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/05 19:44:17 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
	wait_set_time(philo->info->time_die);
	pthread_mutex_unlock(philo->right_fork);
	philo_msg(philo, DIED);
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
	int				philo_nr;
	long			*time;

	philo = (t_philosopher *)threadstruct;
	philo_nr = philo->id;
	if (philo->info->philos_count == 1)
		return (single_philo(philo));
	time = &philo->last_meal_time;
	philo->last_meal_time = get_current_time_ms();
	while (check_for_death_and_eat_limit(philo) == false)
	{
		if (!take_forks(philo) || check_if_should_die(philo))
			break ;
		if (!start_eating(philo) || check_if_should_die(philo))
			break ;
		if (!start_sleeping(philo) || check_if_should_die(philo))
			break ;
		if (!start_thinking(philo) || check_if_should_die(philo))
			break ;
	}
	return (NULL);
}
