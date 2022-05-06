/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:47:32 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/06 12:22:55 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <sys/time.h>
#include <unistd.h>

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

void	wait_set_time(int time_to_wait_in_ms)
{
	long	start_time;
	long	current_time;

	start_time = get_current_time_ms();
	while (1)
	{
		current_time = get_current_time_ms();
		if (current_time - start_time >= time_to_wait_in_ms)
			return ;
		usleep(250);
	}
}
