/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 12:07:29 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/12 14:10:55 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
	* Function to display the correct input parameters when launching the program
	* from a terminal.
	* If a string is given it will display it before the standard error message.
	* @param *error A string to be displayed before the standard error messages.
	* Can be set to NULL.
	* @return 0
*/
static int	parsing_error(char *error)
{
	if (error)
		printf("%s", error);
	printf("./philo [number_of_philosophers] [time_to_die] ");
	printf("[time_to_eat] [time_to_sleep]\n");
	printf("Optional: [number_of_times_each_philosopher_must_eat]\n");
	return (0);
}

/*
	* Custom atoi: Only handles positive integers.
	* @param *str Pointer to the string to be converted to an integer.
	* @return If the string only consists of numbers and does not reach
	* higher then the INT_MAX it will return a converted int.
	* If the input is incorrect it will return -1.
*/
static int	ft_atoi(char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	while (str[i] != '\0' && (str[i] >= '0' && str[i] <= '9'))
	{
		if (i > 10)
			return (-1);
		num *= 10;
		num += str[i] - '0';
		i++;
	}
	if (str[i] != '\0' || num > INT_MAX)
		return (-1);
	return ((int)num);
}

/*
	* Function to parse the input and set
	* the values inside the given info struct.
	* @param argc Argument count from the terminal.
	* @param **argv 2D array of the input given from the terminal.
	* @param *info a pointer to the info struct where the data will be set.
	* @return On correct input [1] else [0].
*/
int	parse_input(int argc, char **argv, t_info *info)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
		return (parsing_error("You do not have enough arguments\n"));
	info->philos_count = ft_atoi(argv[i++]);
	info->time_die = ft_atoi(argv[i++]);
	info->time_eat = ft_atoi(argv[i++]);
	info->time_sleep = ft_atoi(argv[i++]);
	info->philo_has_died = false;
	if (argc == 5)
		info->eat_limit_on = false;
	else
	{
		info->min_times_to_eat = ft_atoi(argv[i]);
		info->eat_limit_on = true;
	}
	if (info->philos_count <= 0 || info->time_die <= 0
		|| info->time_eat <= 0 || info->time_sleep <= 0
		|| (info->eat_limit_on == true && info->min_times_to_eat <= 0))
		return (parsing_error("Please use an integer thats above 0\n"));
	return (1);
}
