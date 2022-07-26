/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tasks.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 18:00:18 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:24:01 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>

/**
 * @brief			Function to print a message.
 * 					Colors can be turned on or off in the headerfile.
 * @param time		The time of the message.
 * @param id		The id of the philosopher sending the message.
 * @param action	The action or message to be printed.
 * @param info		Pointer to the info struct.
 * @return			N/A
*/
void	print_msg(int time, int id, int action, t_info *info)
{
	static const char	*msg[] = {
	[EATING] = "is eating",
	[TAKE_FORK] = "has taken a fork",
	[SLEEPING] = "is sleeping",
	[THINKING] = "is thinking",
	[DIED] = "\033[31mdied\033[0m"
	};
	static const char	*color[] = {
	[RED] = "\033[31m",
	[GREEN] = "\033[32m",
	[YELLOW] = "\033[33m",
	[BLUE] = "\033[34m",
	[MAGENTA] = "\033[35m",
	[CYAN] = "\033[36m",
	[WHITE] = "\033[37m"
	};

	sem_wait(info->print);
	if (USE_COLORS)
		printf("%s%d %d %s\033[0m\n", color[id % 7], time, \
										id + 1, msg[action]);
	else
		printf("%d %d %s\n", time, id + 1, msg[action]);
	if (action != DIED)
		sem_post(info->print);
}

/**
 * @brief		Function to simulate taking 2 forks from the pile.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
void	take_forks(t_info *info)
{
	sem_wait(info->forks);
	print_msg(time_since_start(info), info->id, TAKE_FORK, info);
	sem_wait(info->forks);
	print_msg(time_since_start(info), info->id, TAKE_FORK, info);
}

/**
 * @brief		Function to simulate eating, updates the amount of meals
 * 				eaten and the last_meal_time.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
void	start_eating(t_info *info)
{
	sem_wait(info->data[info->id].checking_sem);
	info->data[info->id].last_meal_time = get_current_time_ms();
	if (info->eat_limit_on == true)
		info->data[info->id].meals_eaten += 1;
	sem_post(info->data[info->id].checking_sem);
	print_msg(time_since_start(info), info->id, EATING, info);
	wait_set_time(info->time_eat);
	sem_post(info->forks);
	sem_post(info->forks);
}

/**
 * @brief		Function to simulate sleeping.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
void	start_sleeping(t_info *info)
{
	print_msg(time_since_start(info), info->id, SLEEPING, info);
	wait_set_time(info->time_sleep);
}

/**
 * @brief		Function to simulate thinking.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
void	start_thinking(t_info *info)
{
	print_msg(time_since_start(info), info->id, THINKING, info);
}
