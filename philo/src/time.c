/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:47:32 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/10 13:02:15 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

long	get_current_time_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

int	time_since_start(t_philosopher *philo)
{
	long	current_time;

	current_time = get_current_time_ms();
	return (current_time - philo->info->start_time);
}

void	wait_set_time(int time_to_wait_in_ms, t_philosopher *philo)
{
	long	start_time;

	start_time = get_current_time_ms();
	while ((get_current_time_ms() - start_time) < time_to_wait_in_ms)
	{
		if (philo)
		{
			if (check_for_death_and_eat_limit(philo) == true)
				return ;
		}
		usleep(250);
	}
}