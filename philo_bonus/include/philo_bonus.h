/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_bonus.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rvan-mee <rvan-mee@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/14 17:09:02 by rvan-mee      #+#    #+#                 */
/*   Updated: 2022/07/26 16:33:01 by rvan-mee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef  PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdbool.h>
# include <semaphore.h>
# include <stdint.h>

# define SUCCESS 0
# define DEATH 1
# define ERROR 2

# define MALLOC_FAIL "Malloc failed\n"
# define FORK_FAIL "Fork failed\n"
# define SEM_FAIL "Failed to create semaphore\n"
# define SEM_EAT_LIM "/philo_eat_limit"
# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"

# define USE_COLORS 1

typedef enum e_color
{
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	RESET
}	t_color;

typedef enum e_actions
{
	EATING,
	TAKE_FORK,
	SLEEPING,
	THINKING,
	DIED
}	t_actions;

typedef struct philo_data_s {
	int32_t		meals_eaten;
	int64_t		last_meal_time;
	int64_t		time_at_death;

	bool		has_posted;
	sem_t		*checking_sem;
	char		checking_sem_name[12];

	int64_t		time_till_start;
	pthread_t	death_check_thread;
}	t_philo_data;

typedef struct info_s {
	uint32_t		philos_count;
	int64_t			start_time;

	int32_t			time_die;
	int32_t			time_eat;
	int32_t			time_sleep;

	bool			eat_limit_on;
	int32_t			min_times_to_eat;
	int32_t			id;

	pid_t			*philos;
	sem_t			*forks;

	pthread_t		saturation_thread;
	sem_t			*eat_limit_sem;

	sem_t			*print;

	t_philo_data	*data;
}	t_info;

/* 				parsing.c	 			*/
void			parse_input(int32_t argc, char **argv, t_info *info);

/*				error.c					*/
void			exit_with_error(char *error, int32_t exit_code);

/*				signals.c				*/
void			execute_children(char *str, t_info *info, \
								uint32_t created_philos, int32_t exit_code);

/*				philo.c					*/
void			philo(t_info *info);
void			check_death_and_eat_limit(t_info *info);

/*				semaphore.c				*/
void			create_semaphores(t_info *info);

/*				init.c					*/
void			init_philo_data(t_info *info);

/*				time.c					*/
int64_t			get_current_time_ms(void);
int				time_since_start(t_info *info);
void			wait_set_time(int32_t time_to_wait_in_ms);
void			wait_till_set_time(int64_t end_time);

/*				tasks.c					*/
void			start_thinking(t_info *info);
void			start_sleeping(t_info *info);
void			start_eating(t_info *info);
void			take_forks(t_info *info);
void			print_msg(int time, int id, int action, t_info *info);

#endif
