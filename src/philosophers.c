/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:40:12 by hheng             #+#    #+#             */
/*   Updated: 2024/12/05 14:40:12 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	handle_fork_pickup(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->fork[0]);
		print_status(philo, "has taken a fork", false, "\033[0m");
		pthread_mutex_lock(philo->fork[1]);
	}
	else
	{
		pthread_mutex_lock(philo->fork[1]);
		print_status(philo, "has taken a fork", false, "\033[0m");
		pthread_mutex_lock(philo->fork[0]);
	}
	print_status(philo, "has taken a fork", false, "\033[0m");
}

static void	update_and_eat(t_philo *philo)
{
	print_status(philo, "is eating", false, "\033[0;32m");
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	if (sim_stopped(philo->table) == false)
		philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	pthread_mutex_unlock(philo->fork[1]);
	pthread_mutex_unlock(philo->fork[0]);
}

static void	*single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->fork[0]);
	print_status(philo, "has taken a fork", false, "\033[0m");
	philo_sleep(philo->table, philo->table->time_to_die);
	print_status(philo, "has died", false, "\033[0;31m");
	pthread_mutex_unlock(philo->fork[0]);
	return (NULL);
}

static void	do_philo_cycle(t_philo *philo)
{
	while (sim_stopped(philo->table) == false)
	{
		handle_fork_pickup(philo);
		update_and_eat(philo);
		print_status(philo, "is sleeping", false, "\033[0m");
		philo_sleep(philo->table, philo->table->time_to_sleep);
		print_status(philo, "is thinking", false, "\033[0m");
	}
}

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
		usleep(philo->table->time_to_eat * 1000 / 2);
	do_philo_cycle(philo);
	return (NULL);
}
