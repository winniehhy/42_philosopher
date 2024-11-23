/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:27:57 by hheng             #+#    #+#             */
/*   Updated: 2024/11/21 14:27:57 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
 start the simulation by initializing threads for each philosopher 
   and a death monitor thread (if more than one philosopher exists).
   * 20ms to ensure all threads are initialized before the simulation starts 
*/
int	start_sim(t_table *table)
{
	int	i;

	table->start_time = get_time_in_ms() + (table->nb_philo * 20);
	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_create(&table->philos[i].thread,
				NULL, &philosopher, &table->philos[i]) != 0)
			return (printf("Could not create thread\n"), 1);
		i++;
	}
	if (table->nb_philo > 1)
	{
		if (pthread_create(&table->death_monitor, NULL, &death_monitor, table))
			return (printf("Could not create thread\n"), 1);
	}
	return (0);
}

/*
	end stimulation by joining all threads and freeing memory
*/
void	end_sim(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	if (table->nb_philo > 1)
		pthread_join(table->death_monitor, NULL);
	destroy_mutexes(table);
	free_table(table);
}

//  ./philo  nb_philo, die, eat, sleep must_eatr_must
// check global : cppcheck --enable=all --inconclusive --check-config .
// remove color code : sed -r "s/\x1B\[[0-9;]*m//g" | grep . > test.txt

int	main(int ac, char *av[])
{
	t_table	table;

	if (ac < 5 || ac > 6)
		return (printf("Incorrect number of arguments\n"), 1);
	if (!is_valid_input(ac, av))
		return (1);
	if (init_table(&table, ac, av) != 0)
		return (1);
	if (start_sim(&table) != 0)
		return (1);
	end_sim(&table);
	return (0);
}
