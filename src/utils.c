/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:40:37 by hheng             #+#    #+#             */
/*   Updated: 2024/12/01
 14:40:37 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_in_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/*
* - Lock the write mutex to ensure only one thread prints at a time
* - Check if the simulation has already stopped && not death.
* - Unlock the mutex and exit
* - Print the status message
*/
void	print_status(t_philo *philo, char *str, bool death_status, char *color)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (sim_stopped(philo->table) == true && death_status == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	printf("%s", color);
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
	printf("\033[0m");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	free_table(t_table *table)
{
	if (!table)
		return ;
	if (table->forks != NULL)
		free(table->forks);
	if (table->philos != NULL)
		free(table->philos);
	table = NULL;
}

/*
* while 
* - Iterate all philosophers to destroy their respective mutexes
* - Destroy the mutex associated with each fork
* - Destroy mutex protecting each philosopher's last meal time & meal count.
* 
*  Destroy the mutex used for thread-safe printing of status messages
*  Destroy the mutex used to manage the simulation's end state
*/
void	destroy_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i].meal_time_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_end_lock);
}

/*
* let say 2ms to eat,  3ms to sleep
* after 2ms eat,(100ms  to 102ms)
* then sleep ; to wakeuptime = 102ms + 3ms = 105ms
* then think, prepare for next eat
*/
void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;
	time_t	current_time;

	wake_up = get_time_in_ms() + sleep_time;
	while (1)
	{
		current_time = get_time_in_ms();
		if (current_time >= wake_up)
			break ;
		if (sim_stopped(table) == true)
			break ;
		if (wake_up - current_time > 10)
			usleep(100);
		else
			usleep(10);
	}
}
