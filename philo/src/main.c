/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:54:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/13 14:59:39 by rvan-mee      ########   odam.nl         */
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
	* Function to destroythe assigned mutexes.
	* After destroying the mutexes it will free the
	* philosophers, fork mutexes and the thread array.
	* @param *info Pointer to the into struct.
	* @param *philos Pointer to an array of all philosophers.
	* @param return_value Integer resembling the return value of this function.
	* @return [return_value]
*/
int	join_threads_and_free_data(t_info *info,
		t_philosopher *philos, pthread_t *threads, int return_value)
{
	int	i;
	int	thread_count;

	i = 0;
	pthread_mutex_lock(&info->creation_check);
	thread_count = info->created_threads;
	pthread_mutex_unlock(&info->creation_check);
	if (join_threads(threads, thread_count) == false)
		return (return_value);
	pthread_mutex_destroy(&info->death_check_mutex);
	pthread_mutex_destroy(&info->creation_check);
	while (i < info->philos_count)
	{
		pthread_mutex_destroy(&info->fork_mutex[i]);
		pthread_mutex_destroy(&philos[i].meal_time_mutex);
		if (info->eat_limit_on == true)
			pthread_mutex_destroy(&philos[i].eat_mutex);
		i++;
	}
	free(philos);
	free(info->fork_mutex);
	free(threads);
	return (return_value);
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
	pthread_mutex_lock(&info->creation_check);
	info->created_threads = 0;
	while (i < philos_count)
	{
		if (pthread_create(&thread_pool[i], NULL,
				&philosopher, &philos[i]) != 0)
		{
			pthread_mutex_unlock(&info->creation_check);
			printf("Thread creation error\n");
			return (false);
		}
		i++;
		info->created_threads = i;
		usleep(250);
	}
	info->start_time = get_current_time_ms();
	pthread_mutex_unlock(&info->creation_check);
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
		return (join_threads_and_free_data(&info, philos, thread_arr, 1));
	monitor_philos(&info, philos);
	return (join_threads_and_free_data(&info, philos, thread_arr, 0));
}
