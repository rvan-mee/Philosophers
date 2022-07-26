/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 14:22:52 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:07:08 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief		Function to check if a philo has died
 * 				or if it has reached it min_times_to_eat limit.
 * @param info	Pointer to the info struct.
 * @return		N/A / Exit if the philosopher has died
*/
void	check_death_and_eat_limit(t_info *info)
{
	int64_t	current_time;

	sem_wait(info->data[info->id].checking_sem);
	current_time = get_current_time_ms();
	if ((info->eat_limit_on == true) && \
		(info->data[info->id].has_posted == false) && \
		(info->data[info->id].meals_eaten >= info->min_times_to_eat))
	{
		sem_post(info->eat_limit_sem);
		info->data[info->id].has_posted = true;
	}
	if (current_time - info->data[info->id].last_meal_time > info->time_die)
	{
		print_msg(time_since_start(info), info->id, DIED, info);
		exit(DEATH);
	}	
	sem_post(info->data[info->id].checking_sem);
}

/**
 * @brief				Function to loop and keep track of the philosophers
 * 						data like last time eaten and amount of meals eaten.
 * @param threadstruct	Pointer to the info struct.
 * @return				N/A
*/
static void	*death_check_thread(void *threadstruct)
{
	t_info	*info;
	int64_t	last_meal_time;

	info = (t_info *)threadstruct;
	sem_wait(info->data[info->id].checking_sem);
	last_meal_time = info->data[info->id].last_meal_time;
	sem_post(info->data[info->id].checking_sem);
	wait_set_time(last_meal_time + info->time_die \
				- get_current_time_ms() - 1);
	while (1)
	{
		check_death_and_eat_limit(info);
		sem_wait(info->data[info->id].checking_sem);
		last_meal_time = info->data[info->id].last_meal_time;
		sem_post(info->data[info->id].checking_sem);
		wait_set_time(last_meal_time + info->time_die \
				- get_current_time_ms() - 1);
	}
}

/**
 * @brief		Function to initialize the last meal time.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
static void	init_last_meal_time(t_info *info)
{
	t_philo_data	*data;

	data = &info->data[info->id];
	sem_wait(data->checking_sem);
	data->last_meal_time = info->start_time;
	sem_post(data->checking_sem);
}

/**
 * @brief		Function to create and detach the death checking thread.
 * @param info	Pointer to the info struct.
 * @return		N/A / Exit if the thread functions fail.
*/
static void	create_death_checker_thread(t_info *info)
{
	if (pthread_create(&info->data[info->id].death_check_thread, \
					NULL, &death_check_thread, info) != 0)
	{
		printf("Failed to create death check thread\n");
		exit(ERROR);
	}
	if (pthread_detach(info->data[info->id].death_check_thread) != 0)
	{
		printf("Failed to detatch death check thread\n");
		exit(ERROR);
	}
}

/**
 * @brief		Function to resemble the philosopher inside the simulation.
 * @param info	Pointer to the info struct.
 * @return		N/A / Exit if the philo has died
*/
void	philo(t_info *info)
{	
	init_last_meal_time(info);
	create_death_checker_thread(info);
	wait_set_time(info->start_time - get_current_time_ms());
	start_thinking(info);
	if (info->id % 2 == 0)
		wait_set_time(info->time_eat / 2);
	while (1)
	{
		take_forks(info);
		start_eating(info);
		start_sleeping(info);
		start_thinking(info);
	}
}
