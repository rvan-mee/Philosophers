/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philos.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:44:23 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/10 18:04:05 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

bool	set_philo_values(t_info *info, t_philosopher *philos, int i)
{
	philos[i].info = info;
	philos[i].id = i;
	philos[i].left = i;
	philos[i].right = (i + 1) % info->philos_count;
	philos[i].meals_eaten = 0;
	philos[i].last_meal_time = get_current_time_ms();
	return (true);
}

void	philo_msg(t_philosopher *philo, int msg)
{
	int	time;

	time = time_since_start(philo);
	if (check_for_death_and_eat_limit(philo))
		return ;
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
	pthread_mutex_lock(&philo->info->fork_mutex[philo->right]);
	wait_set_time(philo->info->time_die, philo);
	pthread_mutex_unlock(&philo->info->fork_mutex[philo->right]);
	pthread_mutex_destroy(&philo->info->fork_mutex[philo->right]);
	if (philo->info->eat_limit_on == true)
		pthread_mutex_destroy(&philo->eat_mutex);
	free(philo->info->fork_mutex);
	return (NULL);
}

void	*philosopher(void *threadstruct)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)threadstruct;
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = get_current_time_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (philo->info->philos_count == 1)
		return (single_philo(philo));
	if (philo->id % 2 == 1)
		usleep(250);
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
