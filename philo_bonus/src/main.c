/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 16:33:27 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:18:39 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

/**
 * @brief 		Function to create all philosopher proccesses.
 * @param info	Pointer to the info struct.
 * @return 		N/A / Exit if fork fails
*/
static void	create_philosophers(t_info *info)
{
	size_t	i;
	pid_t	pid;

	i = 0;
	info->philos = malloc(sizeof(pid_t) * info->philos_count);
	if (!info->philos)
		exit_with_error(MALLOC_FAIL, ERROR);
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

/**
 * @brief		Function to wait for a proccess to end, if it does it will
 * 				kill all other child proccesses and exit.
 * @param info	Pointer to the info struct.
*/
static void	wait_for_philos(t_info *info)
{
	waitpid(-1, NULL, 0);
	execute_children(NULL, info, info->philos_count, DEATH);
}

/**
 * @brief				Function to keep track of the amount of philosophers
 * 						that have passed the min_times_to_eat. Once all
 * 						philosophers have passed it the simulation will end.
 * @param threadstruct	Pointer to the info struct.
 * @return				Exit if all philos have eaten enough.
*/
static void	*check_saturated_philos(void *threadstruct)
{
	t_info		*info;
	uint32_t	ate_enough;

	ate_enough = 0;
	info = (t_info *)threadstruct;
	while (ate_enough < info->philos_count)
	{	
		sem_wait(info->eat_limit_sem);
		ate_enough++;
	}
	sem_wait(info->print);
	printf("\033[32mAll philosophers have \
eaten enough, stopping simulation..\n\033[0m");
	execute_children(NULL, info, info->philos_count, SUCCESS);
	return (NULL);
}

/**
 * @brief		Function to create and detatch the thread that checks
 * 				if all philosophers have eaten min_times_to_eat times
 * 				if the eat limit is enabled.
 * @param info	Pointer to the info struct.
 * @return		N/A
*/
static void	create_saturation_thread(t_info *info)
{
	if (pthread_create(&info->saturation_thread, NULL, \
						&check_saturated_philos, info) != 0)
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

/**
 * @brief	Starts off with creating and initializing
 * 			all philosophers and data.
 * 			Will turn into a death monitoring state after everything
 * 			is initialized and started.
*/
int32_t	main(int32_t argc, char **argv)
{
	t_info	info;

	parse_input(argc, argv, &info);
	init_philo_data(&info);
	create_semaphores(&info);
	create_philosophers(&info);
	if (info.eat_limit_on)
		create_saturation_thread(&info);
	wait_for_philos(&info);
	return (0);
}
