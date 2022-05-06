/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:54:54 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/05 18:27:57 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "philos.h"

static int	join_threads(pthread_t *thread_pool, int thread_count)
{
	int	i;

	i = 0;
	while (i < thread_count)
	{
		if (pthread_join(thread_pool[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	create_threads(pthread_t *thread_pool, int philos_count, t_philosopher *philos)
{
	int	i;

	i = 0;
	while (i < philos_count)
	{
		if (i % 2 == 0 || i == philos_count - 1)
			usleep(100);
		if (pthread_create(&thread_pool[i], NULL, &philosopher, &philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	free_mallocs_main(pthread_t *threads, t_philosopher *philos, pthread_mutex_t *mutex)
{
	write(1, "malloc error!\n", 14);
	if (threads)
		free(threads);
	if (philos)
		free(philos);
	if (mutex)
	{
		pthread_mutex_destroy(mutex);
		free(mutex);
	}
}

static bool	set_meal_data(t_info *info)
{
	pthread_mutex_t	*new_mutex;
	int				i;

	i = 0;
	if (info->eat_limit_on == false)
		return (true);
	info->meals_eaten = malloc(sizeof(int) * info->philos_count);
	if (!info->meals_eaten)
		return (false);
	while (i < info->philos_count)
	{
		info->meals_eaten[i] = 0;
		i++;
	}
	new_mutex = get_new_mutex();
	if (!new_mutex)
	{
		free(info->meals_eaten);
		return (false);
	}
	info->eat_mutex = new_mutex;
	return (true);
}

int	main(int argc, char **argv)
{
	pthread_t		*thread_arr;
	t_philosopher	*philos;
	t_info			info;

	setbuf(stdout, NULL);


	info.start_time = get_current_time_ms();
	if (!parse_input(argc, argv, &info))
		return (1);
	thread_arr = malloc(info.philos_count * sizeof(pthread_t));
	philos = malloc(info.philos_count * sizeof(t_philosopher));
	info.death_check_mutex = get_new_mutex();
	if (thread_arr == NULL || philos == NULL || info.death_check_mutex == NULL)
	{
		free_mallocs_main(thread_arr, philos, info.death_check_mutex);
		return (1);
	}
	if (set_meal_data(&info) == false)
		return (1);
	if (init_philos(&info, philos) == false)
		return (1);
	create_threads(thread_arr, info.philos_count, philos);
	// error check ?
	join_threads(thread_arr, info.philos_count);
	// error check ?
	if (info.philos_count > 1)
		destroy_forks(&info, philos);
	// system("leaks philo");
	return (0);
}
