/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:38:41 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:38:41 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
* lock mutex, update status  (true = end, false = ongoing), unlock mutex
*/
static void	set_end_status(t_table *table, bool status)
{
	pthread_mutex_lock(&table->sim_end_lock);
	table->sim_end = status;
	pthread_mutex_unlock(&table->sim_end_lock);
}

/*
 * check if the philosopher has died
 * if the time since the last meal is > time to die
 * print the status of the philosopher
 * unlock the meal_time_lock
*/
static bool	kill_philo(t_philo *philo)
{
    time_t	current_time;
    time_t  buffer = 1; // Small buffer to prevent race conditions

    current_time = get_time_in_ms();
    if ((current_time - philo->last_meal) >= (philo->table->time_to_die - buffer))
    {
        set_end_status(philo->table, true);
        print_status(philo, "has died", true, "\033[0;31m");
        pthread_mutex_unlock(&philo->meal_time_lock);
        return (true);
    }
    return (false);
}

/*
* - Assume all philosophers have eaten enough at the start.
*   While Lock philosopher's meal time mutex;
*  1. Checks if any philosopher has not eaten within the time_to_die
*  2. Check if the philosopher has not yet reached the required number of meals
*  
*   True = end simulation; False = continue simulation
*/
static bool	end_condition(t_table *table)
{
	int		i;
	bool	all_ate_enough;

	i = 0;
	all_ate_enough = true;
	while (i < table->nb_philo)
	{
		pthread_mutex_lock(&table->philos[i].meal_time_lock);
		if (kill_philo(&table->philos[i]) == true)
			return (true);
		if (table->must_eat_count != -1
			&& table->philos[i].eat_count < table->must_eat_count)
			all_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i].meal_time_lock);
		i++;
	}
	if (table->must_eat_count != -1 && all_ate_enough == true)
		return (set_end_status(table, true), true);
	return (false);
}

/*
* update simulation_end to true, then unlock
*/
bool	sim_stopped(t_table *table)
{
	bool	end;

	end = false;
	pthread_mutex_lock(&table->sim_end_lock);
	if (table->sim_end == true)
		end = true;
	pthread_mutex_unlock(&table->sim_end_lock);
	return (end);
}

/*
* Must_eat_count = 0, exit immediately
* check end conditin has been met, true = exit
* thread sleeps for 100 microseconds before rechecking
*/
void	*death_monitor(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		return (NULL);
	set_end_status(table, false);
	while (get_time_in_ms() < table->start_time)
		continue ;
	while (1)
	{
		if (end_condition(table) == true)
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
