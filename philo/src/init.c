/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:52:42 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/13 14:22:21 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/*
	* Function to inititate the values set inside the info struct.
	* @param *info Pointer to the info struct.
	* @return If successful [true] else [false]
*/
bool	init_info(t_info *info)
{
	info->fork_mutex = malloc(sizeof(pthread_mutex_t) * info->philos_count);
	if (!info->fork_mutex)
		return (false);
	if (pthread_mutex_init(&info->death_check_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&info->creation_check, NULL) != 0)
	{
		pthread_mutex_destroy(&info->death_check_mutex);
		return (false);
	}
	return (true);
}

/*
	* Function to destory the created mutexes.
	* @param *philos Array of philosophers.
	* @param meals Amount of meal mutexes created.
	* @param forks Amount of fork mutexes created.
	* @param eat Amound of eat mutexes created.
	* @return [false]
*/
static bool	destroy_created_mutexes(t_philosopher *philos, int meals,
		int forks, int eat)
{
	int	i;

	i = 0;
	while (i < meals)
	{
		pthread_mutex_destroy(&philos[i].meal_time_mutex);
		i++;
	}
	i = 0;
	while (i < forks)
	{
		pthread_mutex_destroy(&philos[0].info->fork_mutex[i]);
		i++;
	}
	i = 0;
	if (philos->info->eat_limit_on == true)
	{
		while (i < eat)
		{
			pthread_mutex_destroy(&philos[i].eat_mutex);
			i++;
		}
	}
	return (false);
}

/*
	* Function to inititate the values set inside all philosophers
	* and the info struct.
	* @param *info Pointer to the info struct.
	* @param *philos Pointer to the array of philosophers.
	* @return If successful [true] else [false]
*/
bool	init_philos(t_info *info, t_philosopher *philos)
{
	int	forks;
	int	eat;
	int	i;

	i = 0;
	eat = 0;
	forks = 0;
	while (i < info->philos_count)
	{
		set_philo_values(info, philos, i);
		if (pthread_mutex_init(&info->fork_mutex[i], NULL) != 0)
			return (destroy_created_mutexes(philos, i, forks, eat));
		forks++;
		if (pthread_mutex_init(&philos[i].meal_time_mutex, NULL) != 0)
			return (destroy_created_mutexes(philos, i + 1, forks, eat));
		i++;
		if (philos->info->eat_limit_on == true)
		{
			if (pthread_mutex_init(&philos[i - 1].eat_mutex, NULL) != 0)
				return (destroy_created_mutexes(philos, i, forks, eat));
			eat++;
		}
	}
	return (true);
}
