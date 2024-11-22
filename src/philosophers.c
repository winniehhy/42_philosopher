/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:40:12 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:40:12 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* wait until stimulation start time
*/
static void	sim_start_wait(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}

/*
* let say 2ms to eat,  3ms to sleep
* after 2ms eat,(100ms  to 102ms)
* then sleep ; to wakeuptime = 102ms + 3ms = 105ms
* then think, prepare for next eat
*/
static void	philo_sleep(t_table *table, time_t sleep_time)
{
	time_t	wake_up;

	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (sim_stopped(table) == true)
			break ;
		usleep(100);
	}
}
/*
* 1 philosopher case
* take left fork, sleep until dead time
*/
static void	*single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[0]);
	print_status(philo, "has taken a fork", false);
	philo_sleep(philo->table, philo->table->time_to_die);
	print_status(philo, "has died", false);
	pthread_mutex_unlock(philo->fork[1]);
	return (NULL);
}
/*
* fork[0]= left fork, fork[1]= right fork
* pthread_mutex_lock(&philo->meal_time_lock); = update last meal time
*
* When even eating :
* - lock left right fork
* - odd sleep for time_to_eat
*
* After eating :
* - update last meal time, eat count +1
* - unlock forks
* - sleep for time_to_sleep
* - think
*/
static void	eat_sleep_think(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[0]);
	print_status(philo, "has taken a fork", false);
	pthread_mutex_lock(philo->fork[1]);
	print_status(philo, "has taken a fork", false);
	print_status(philo, "is eating", false);
	philo_sleep(philo->table, philo->table->time_to_eat);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	if (sim_stopped(philo->table) == false)
		philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_time_lock);
	print_status(philo, "is sleeping", false);
	pthread_mutex_unlock(philo->fork[1]);
	pthread_mutex_unlock(philo->fork[0]);
	philo_sleep(philo->table, philo->table->time_to_sleep);
	print_status(philo, "is thinking", false);
}

/*
* lock when philo easting, unlock after eating
* philo die if last meal > time to die
* (philo->id % 2) = id is odd, sleep 5ms
* while stimulation is not stopped-> continue eat, sleep, think
*/
void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_wait(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	if (philo->table->nb_philo == 1)
		return (single_philo_routine(philo));
	else if (philo->id % 2)
		usleep(5000);
	while (sim_stopped(philo->table) == false)
		eat_sleep_think(philo);
	return (NULL);
}
