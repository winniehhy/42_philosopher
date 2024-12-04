/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hheng < hheng@student.42kl.edu.my>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 14:39:18 by hheng             #+#    #+#             */
/*   Updated: 2024/12/05 14:39:18 by hheng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_only_digits(char *str)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		str++;
	}
	return (true);
}

int	philo_atoi(char *str)
{
	unsigned long	nbr;
	int				i;

	i = 0;
	nbr = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (nbr > INT_MAX)
		return (-1);
	return ((int)nbr);
}

bool	is_valid_input(int ac, char **av)
{
	int	i;
	int	nbr;

	i = 0;
	while (++i < ac)
	{
		if (!is_only_digits(av[i]))
			return (printf("Arguments should only consist of digits\n"), false);
		nbr = philo_atoi(av[i]);
		if (i == 1 && (nbr <= 0 || nbr > PHILO_MAX))
		{
			printf("Number of philosophers should be between 1 and 200\n");
			return (false);
		}
		if (i != 1 && nbr == -1)
			return (printf("Invalid argument\n"), false);
	}
	return (true);
}

/*
* wait until stimulation start time
*/
void	sim_start_wait(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
