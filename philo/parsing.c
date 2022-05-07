/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 12:07:29 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/07 17:20:33 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static int	parsing_error(char *error)
{
	if (error)
		write(1, error, ft_strlen(error));
	write(1, "./philo ", 9);
	write(1, "[number_of_philosophers] ", 25);
	write(1, "[time_to_die] ", 14);
	write(1, "[time_to_eat] ", 14);
	write(1, "[time_to_sleep]\n", 16);
	write(1, "Optional: [number_of_times_each_philosopher_must_eat]\n", 54);
	return (0);
}

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
	if (info->philos_count <= 0 || info->time_die <= 0 || info->time_eat <= 0
		|| info->time_sleep <= 0
		|| (info->eat_limit_on == true && info->min_times_to_eat <= 0))
		return (parsing_error("Please use an integer thats above 0\n"));
	return (1);
}
