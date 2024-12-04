/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:35:19 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:35:19 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>
# include <stdio.h>

# define PHILO_MAX 200

typedef struct s_table	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eat_count;
	pthread_mutex_t	*fork[2];
	pthread_mutex_t	meal_time_lock;
	time_t			last_meal;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				nb_philo;
	pthread_t		death_monitor;
	time_t			start_time;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				must_eat_count;
	bool			sim_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	sim_end_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
}	t_table;

/*Parsing*/

int		philo_atoi(char *str);
bool	is_valid_input(int ac, char **av);
void	sim_start_wait(time_t start_time);

/*Init*/

int		init_table(t_table *table, int ac, char **av);

/*Simulation*/

int		start_sim(t_table *table);
void	end_sim(t_table *table);
bool	sim_stopped(t_table *table);

/*Utils*/

time_t	get_time_in_ms(void);
void	print_status(t_philo *philo, char *str, bool death_status, char *color);
void	free_table(t_table *table);
void	destroy_mutexes(t_table *table);
void	philo_sleep(t_table *table, time_t sleep_time);

/*Routines*/

void	*philosopher(void *data);
void	*death_monitor(void *data);

#endif