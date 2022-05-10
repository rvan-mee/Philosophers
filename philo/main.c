/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:54:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/10 16:53:45 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "philos.h"

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

static bool	create_threads(pthread_t *thread_pool,
	int philos_count, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < philos_count)
	{
		if (pthread_create(&thread_pool[i], NULL,
				&philosopher, &philos[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

static void	free_mallocs_main(pthread_t *threads, t_philosopher *philos)
{
	write(1, "malloc error!\n", 14);
	if (threads)
		free(threads);
	if (philos)
		free(philos);
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
	if (thread_arr == NULL || philos == NULL
		|| pthread_mutex_init(&info.death_check_mutex, NULL) != 0)
	{
		free_mallocs_main(thread_arr, philos);
		return (1);
	}
	if (init_philos(&info, philos) == false)
		return (1);
	info.start_time = get_current_time_ms();
	if (create_threads(thread_arr, info.philos_count, philos) == false)
		return (1);
	monitor_philos(&info, philos);
	if (join_threads(thread_arr, info.philos_count) == false)
		return (1);
	return (destroy_forks(&info, philos));
}
