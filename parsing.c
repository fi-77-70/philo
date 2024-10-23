/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:42:32 by filferna          #+#    #+#             */
/*   Updated: 2024/10/23 11:52:29 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_number(char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			exit(printf("Error, wrong argument\n"));
		str++;
		i++;
		if (i > 10)
			exit(printf("Error, number too big\n"));
	}
	return (1);
}

void	set_forks(t_philo *philo,
			pthread_mutex_t **fork_1, pthread_mutex_t **fork_2)
{
	if (philo->id == 1 && !philo->table->n_philos % 2)
	{
		*fork_1 = philo->r_fork;
		*fork_2 = philo->l_fork;
	}
	if (philo->id % 2)
	{
		*fork_1 = philo->l_fork;
		*fork_2 = philo->r_fork;
	}
	else
	{
		*fork_1 = philo->r_fork;
		*fork_2 = philo->l_fork;
	}
}

int	eat_two(t_philo *philo)
{
	if (checker(philo))
		return (1);
	printf("%ld philo %d, is eating\n",
		get_time() - philo->start_time, philo->id);
	pthread_mutex_lock(*philo->table->checker_mutex);
	philo->eat_times += 1;
	if (philo->eat_times == philo->total_meals)
		philo->table->meals += philo->total_meals;
	philo->last_meal = get_time();
	pthread_mutex_unlock(*philo->table->checker_mutex);
	return (0);
}

void	monitor(t_table *table)
{
	int	i;

	i = 0;
	while (table->some_one_dead == 0)
	{
		pthread_mutex_lock(*table->checker_mutex);
		if (i == table->n_philos)
			i = 0;
		if (table->meals >= table->max_meals && table->max_meals != -1)
			table->some_one_dead = 1;
		else if ((get_time()
				- table->philo[i].last_meal) > table->philo[i].time_to_die)
		{
			table->some_one_dead = 1;
			printf("%ld philo %d, is dead\n",
				get_time() - table->philo[i].start_time, table->philo[i].id);
		}
		i++;
		pthread_mutex_unlock(*table->checker_mutex);
	}
}

int	think(t_philo *philo)
{
	if (checker(philo))
		return (1);
	printf("%ld philo %d, is thinking\n",
		get_time() - philo->start_time, philo->id);
	return (try_eat(philo));
}
