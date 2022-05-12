/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philos.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 11:59:16 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/05/12 15:42:39 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef  PHILOS_H
# define PHILOS_H

# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# define EATING 0
# define TAKE_FORK 1
# define SLEEPING 2
# define THINKING 3
# define DIED 4

/*
	-	Number of philosophers to create.

	-	If a philosopher did not start eating since
		the start of the program or their last meal
		and it has exeeded time to die they die.

	-	Time it takes for a philosopher to eat
		they hold 2 forks during this time.

	-	Time it will spend sleeping.

	-	(Optional) If all philosophers have eaten
		this many times the simulation stops
		if not specified the simulation stops
		when a philosopher dies.
*/
typedef struct info_s {
	long			start_time;
	int				philos_count;

	int				time_die;
	int				time_eat;
	int				time_sleep;

	int				min_times_to_eat;
	bool			eat_limit_on;

	bool			philo_has_died;
	pthread_mutex_t	death_check_mutex;
	pthread_mutex_t	*fork_mutex;
}	t_info;

typedef struct philosopher_s {
	t_info			*info;
	int				left;
	int				right;
	int				id;

	int				meals_eaten;
	pthread_mutex_t	eat_mutex;

	long			last_meal_time;
	pthread_mutex_t	meal_time_mutex;
}	t_philosopher;

/* 				forks.c	 				*/
bool			init_info(t_info *info);
bool			init_philos(t_info *info, t_philosopher *philos);
int				destroy_and_free_mutexes(t_info *info, t_philosopher *philos,
					pthread_t *threads, int return_value);
void			unlock_both_forks(t_philosopher *philo);

/* 				parsing.c	 			*/
int				parse_input(int argc, char **argv, t_info *info);

/* 				philos.c	 			*/
void			set_philo_values(t_info *info, t_philosopher *philos, int i);
bool			display_msg_if_not_dead(t_philosopher *philo, int msg);
void			*philosopher(void *threadstruct);

/*			 	tasks.c	 				*/
bool			take_forks(t_philosopher *philo);
bool			start_eating(t_philosopher *philo);
bool			start_sleeping(t_philosopher *philo);
bool			start_thinking(t_philosopher *philo);
bool			check_if_should_die(t_philosopher *philo);

/*			 	time.c					 */
long			get_current_time_ms(void);
void			wait_set_time(int time_to_wait_in_ms, t_philosopher *philo);
int				time_since_start(t_philosopher *philo);

/* 				utils.c					 */
void			set_philos_dead(t_info *info);		 
void			monitor_philos(t_info *info, t_philosopher *philo);
bool			check_for_death_and_eat_limit(t_philosopher *philo);

#endif
