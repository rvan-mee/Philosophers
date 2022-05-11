/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tasks_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 12:07:51 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/08 16:04:19 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include <stdio.h>

// bool	take_left_fork(t_philosopher *philo)
// {
// 	if (check_for_death_and_eat_limit(philo))
// 		return (false);
// 	pthread_mutex_lock(philo->left_fork);
// 	if (check_for_death_and_eat_limit(philo))
// 	{
// 		pthread_mutex_unlock(philo->left_fork);
// 		return (false);
// 	}
// 	philo_msg(philo, TAKE_FORK);
// 	return (true);
// }

// bool	take_right_fork(t_philosopher *philo)
// {
// 	if (check_for_death_and_eat_limit(philo))
// 		return (false);
// 	pthread_mutex_lock(philo->right_fork);
// 	if (check_for_death_and_eat_limit(philo))
// 	{
// 		pthread_mutex_unlock(philo->right_fork);
// 		return (false);
// 	}
// 	philo_msg(philo, TAKE_FORK);
// 	return (true);
// }
