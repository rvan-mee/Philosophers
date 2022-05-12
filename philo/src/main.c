/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:54:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/12 15:43:00 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "philos.h"

/*
	* Function to join all the threads together
	* and wait for the completion of all of them.
	* @param *thread_pool Pointer to the array of threads.
	* @thread_count Integer resembling the amount of threads there are.
	* @return If all threads joined and ended successfully [true] else [false].
*/
static bool	join_threads(pthread_t *thread_pool, int thread_count)
{
	int	i;

	i = 0;
	while (i < thread_count)
	{
		if (pthread_join(thread_pool[i], NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

/*
	* Function to create and start the threads.
	* @param *thread_pool Pointer to the array of threads.
	* @param philos_count Integer resembling the amount of philosophers.
	* @param *philos Pointer to the array of philosophers.
	* @param *info Pointer to the info struct.
	* @return If all threads were created successfully [true] else [false].
*/
static bool	create_threads(pthread_t *thread_pool,
	int philos_count, t_philosopher *philos, t_info *info)
{
	int	i;

	i = 0;
	info->start_time = get_current_time_ms();
	while (i < philos_count)
	{
		if (pthread_create(&thread_pool[i], NULL,
				&philosopher, &philos[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

/*
	* Function to free the given pointers if they are assigned.
	* Displays an error message before trying to free.
	* @param *threads Pointer to the array of threads.
	* @param *philos Pointer to the array of philosophers.
	* @return [1]
*/
static int	malloc_error(pthread_t *threads, t_philosopher *philos)
{
	printf("Malloc error!\n");
	if (threads)
		free(threads);
	if (philos)
		free(philos);
	return (1);
}

int	main(int argc, char **argv)
{
	pthread_t		*thread_arr;
	t_philosopher	*philos;
	t_info			info;

	if (!parse_input(argc, argv, &info))
		return (1);
	thread_arr = malloc(info.philos_count * sizeof(pthread_t));
	philos = malloc(info.philos_count * sizeof(t_philosopher));
	if (thread_arr == NULL || philos == NULL)
		return (malloc_error(thread_arr, philos));
	if (init_info(&info) == false)
		return (1);
	if (init_philos(&info, philos) == false)
		return (1);
	if (create_threads(thread_arr, info.philos_count, philos, &info) == false)
	{
		set_philos_dead(&info);
		usleep(100000);
		return (destroy_and_free_mutexes(&info, philos, thread_arr, 1));
	}
	monitor_philos(&info, philos);
	if (join_threads(thread_arr, info.philos_count) == false)
		return (destroy_and_free_mutexes(&info, philos, thread_arr, 1));
	return (destroy_and_free_mutexes(&info, philos, thread_arr, 0));
}
