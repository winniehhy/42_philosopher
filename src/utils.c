/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:40:37 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:40:37 by hheng            ###   ########.fr       */
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
* - Check if the simulation has already stopped and the message is not about a death.
* - Unlock the mutex and exit
* - Print the status message
*/
void	print_status(t_philo *philo, char *str, bool death_status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (sim_stopped(philo->table) == true && death_status == false)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
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
* - Iterate through all philosophers to destroy their respective mutexes
* - Destroy the mutex associated with each fork
* - Destroy the mutex protecting each philosopher's last meal time and meal count.
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
