/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 13:53:29 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:29:26 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief		Function to get the currrent time.
 * @param void	N/A.
 * @return		The current time in miliseconds.
*/
int64_t	get_current_time_ms(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}

/**
 * @brief		Function to get the amount of time elapsed since the
 * 				set start value.
 * @param info	Pointer to the info struct.
 * @return		Time elapsed since the start in miliseconds.
*/
int	time_since_start(t_info *info)
{
	int64_t	current_time;

	current_time = get_current_time_ms();
	return (current_time - info->start_time);
}

/**
 * @brief						Custom sleep function to more accuratly sleep.
 * @param time_to_wait_in_ms	Time to sleep in miliseconds.
 * @return						N/A
*/
void	wait_set_time(int time_to_wait_in_ms)
{
	int64_t	curr;
	int64_t	end;
	int64_t	diff;

	end = get_current_time_ms() + time_to_wait_in_ms;
	while (1)
	{
		curr = get_current_time_ms();
		if (curr >= end)
			break ;
		diff = end - curr;
		if (diff <= 1)
		{
			while (get_current_time_ms() < end)
				usleep(200);
			return ;
		}
		usleep(diff * 2 * 1000 / 3);
	}
}
