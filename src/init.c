/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:36:00 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:36:00 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes and allocates memory for forks (mutexes).
 * @param table Pointer to the table structure containing simulation data.
 * @return Pointer to the array of mutexes on success, NULL on failure.
 */
static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!forks)
		return (printf("Could not allocate memory\n"), NULL);
	while (i < table->nb_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (printf("Could not create mutex\n"), NULL);
		i++;
	}
	return (forks);
}

/**
 * @brief Initializes and allocates memory for philosopher data.
 * @param table Pointer to the table structure containing simulation data.
 * @param philos Pointer to the philosopher array to be initialized.
 * @return 0 on success, 1 on failure.
 */
static int	init_philo(t_table *table, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!philos)
		return (printf("Could not allocate memory\n"), 1);
	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_mutex_init(&(*philos)[i].meal_time_lock, NULL) != 0)
			return (printf("Could not create mutex\n"), 1);
		(*philos)[i].id = i;
		(*philos)[i].eat_count = 0;
		(*philos)[i].table = table;
		(*philos)[i].fork[0] = &table->forks[i];
		(*philos)[i].fork[1] = &table->forks[(i + 1) % table->nb_philo];
		i++;
	}
	return (0);
}

/**
 * @brief Initializes all mutexes needed for the simulation.
 * @param table Pointer to the table structure containing simulation data.
 * @return 0 on success, 1 on failure.
 */
static int	init_all_mutexes(t_table *table)
{
	table->forks = init_forks(table);
	if (!table->forks)
		return (1);
	if (pthread_mutex_init(&table->sim_end_lock, NULL) != 0)
		return (printf("Could not create mutex\n"), 1);
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (printf("Could not create mutex\n"), 1);
	return (0);
}

/**
 * @brief Initializes the simulation table with input parameters.
 * @param table Pointer to the table structure to be initialized.
 * @return 0 on success, 1 on failure.
 */
int	init_table(t_table *table, int ac, char **av)
{
	memset(table, 0, sizeof(t_table));
	table->nb_philo = philo_atoi(av[1]);
	table->time_to_die = philo_atoi(av[2]);
	table->time_to_eat = philo_atoi(av[3]);
	table->time_to_sleep = philo_atoi(av[4]);
	table->must_eat_count = -1;
	if (ac == 6)
		table->must_eat_count = philo_atoi(av[5]);
	if (init_all_mutexes(table) != 0)
		return (1);
	if (init_philo(table, &table->philos) != 0)
		return (1);
	return (0);
}
