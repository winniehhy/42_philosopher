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

static void	set_end_status(t_table *table, bool status)
{
	pthread_mutex_lock(&table->sim_end_lock);
	table->sim_end = status;
	pthread_mutex_unlock(&table->sim_end_lock);
}

static bool	kill_philo(t_philo *philo)
{
	time_t	current_time;

	current_time = get_time_in_ms();
	if ((current_time - philo->last_meal) >= philo->table->time_to_die)
	{
		set_end_status(philo->table, true);
		print_status(philo, "died", true);
		pthread_mutex_unlock(&philo->meal_time_lock);
		return (true);
	}
	return (false);
}

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
