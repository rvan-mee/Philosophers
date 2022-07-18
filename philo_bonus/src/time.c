/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 13:53:29 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/18 17:27:05 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

/*
	* Function to get the currrent time.
	* @return The current time in miliseconds.
*/
int64_t	get_current_time_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

/*
	* Function to get the amount of time elapsed since the
	* set start value.
	* @return Time elapsed since the start in miliseconds.
*/
int	time_since_start(t_info *info)
{
	int64_t	current_time;

	current_time = get_current_time_ms();
	return (current_time - info->start_time);
}


void	wait_set_time(int time_to_wait_in_ms, t_info *info)
{
	int64_t	start_time;

	(void)info;
	start_time = get_current_time_ms();
	while ((get_current_time_ms() - start_time) < time_to_wait_in_ms)
	{
		// check_death_and_eat_limit(info);
		usleep(250);
	}
}
