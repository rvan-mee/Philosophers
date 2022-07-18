/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:33:27 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/18 19:57:48 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

static void	create_philosophers(t_info *info)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	info->philos = malloc(sizeof(pid_t) * info->philos_count);
	if (!info->philos)
		exit_with_error(MALLOC_FAIL, info, ERROR);
	info->start_time = get_current_time_ms() + 200;
	while (i < info->philos_count)
	{
		info->id = i;
		pid = fork();
		if (pid == -1)
			execute_children(FORK_FAIL, info, i, ERROR);
		else if (pid == 0)
			philo(info);
		info->philos[i] = pid;
		i++;
	}
}

static void	wait_for_philos(t_info *info)
{
	waitpid(-1, NULL, 0);
	execute_children(NULL, info, info->philos_count, DEATH);
}

static void	*check_saturated_philos(void *threadstruct)
{
	t_info		*info;

	info = (t_info *)threadstruct;
	while (1)
	{
		sem_wait(info->eat_limit_sem);
		info->saturated_philos += 1;
		if (info->saturated_philos == info->philos_count)
			break ;
	}
	execute_children(NULL, info, info->philos_count, SUCCESS);
	return (NULL);
}

void	create_saturation_thread(t_info *info)
{
	if (pthread_create(&info->saturation_thread, NULL, \
						&check_saturated_philos, &info) != 0)
	{
		printf("Failed to create saturation thread\n");
		execute_children(NULL, info, info->philos_count, ERROR);
	}
	if (pthread_detach(info->saturation_thread) != 0)
	{
		printf("Failed to detach saturation thread\n");
		execute_children(NULL, info, info->philos_count, ERROR);
	}
}

int32_t	main(int32_t argc, char **argv)
{
	t_info	info;

	// setbuf(stdout, 0);
	parse_input(argc, argv, &info);
	init_philo_data(&info);
	create_semaphores(&info);
	create_philosophers(&info);
	if (info.eat_limit_on)
		create_saturation_thread(&info);
	wait_for_philos(&info);
	return (0);
}
