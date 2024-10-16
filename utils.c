/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:36:41 by filferna          #+#    #+#             */
/*   Updated: 2024/10/16 11:55:53 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *str)
{
	int sign;
	long result;

	sign = 1;
	result = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while(*str)
	{
		result = result * 10 + (*str - 48);
		str++;
	}
	if (result > INT_MAX)
		exit (printf("Error, number too big\n"));
	return (result * sign);
}

void	init_table(t_table *table, char **av)
{
	table->n_philos = ft_atoi(av[1]);
	table->some_one_dead = 0;
	table->ready = 0;
	table->meals = 0;
	table->checker_mutex = malloc(sizeof(pthread_mutex_t *) * 1);
	table->checker_mutex[0] = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->checker_mutex[0], NULL);
	if (av[5])
		table->max_meals = ft_atoi(av[5]) * table->n_philos;
	else
		table->max_meals = -1;
}

void	init_philos(t_table *table, char **av)
{
	int	i;
	
	i = -1;
	table->philo = malloc(sizeof(t_philo) * table->n_philos);
	table->mutex_original = malloc(sizeof(pthread_mutex_t) * table->n_philos);
	while(++i < table->n_philos)
	{
		table->philo[i].id = i + 1;
		table->philo[i].is_dead = 0;
		table->philo[i].time_to_die = ft_atoi(av[2]);
		table->philo[i].time_to_eat = ft_atoi(av[3]);
		table->philo[i].time_to_sleep = ft_atoi(av[4]);
		table->philo[i].current_time = get_time();
		table->philo[i].table = table;
		pthread_mutex_init(&table->mutex_original[i], NULL);
		table->philo[i].l_fork = &table->mutex_original[i];
		if(i > 0)
		{
			table->philo[i - 1].r_fork = &table->mutex_original[i];
			if(i == (table->n_philos - 1))
				table->philo[i].r_fork = &table->mutex_original[0];
		}
	}
}

void	start_philos(t_table *table)
{
	int	i;
	int	j;
	suseconds_t	time;

	i = -1;
	j = -1;
	time = get_time();
	while (++i < table->n_philos)
	{
		table->philo[i].start_time = time;
		table->philo[i].last_meal = get_time();
		pthread_create(&table->philo[i].th, NULL, &loop, (void *)&table->philo[i]);
	}
	while (table->some_one_dead == 0)
	{
		if (i == table->n_philos)
			i = 0;
		pthread_mutex_lock(*table->checker_mutex);
		if (table->meals >= table->max_meals && table->max_meals != -1)
			table->some_one_dead = 1;
		else if ((get_time() - table->philo[i].last_meal) > table->philo[i].time_to_die)
		{
			printf("%ld philo %d, is dead\n",
				get_time() - table->philo[i].start_time, table->philo[i].id);
			table->some_one_dead = 1;
		}
		pthread_mutex_unlock(*table->checker_mutex);
		i++;
	}
	while (++j < table->n_philos)
		pthread_join(table->philo[j].th, NULL);
}

suseconds_t	get_time(void)
{
	struct timeval current_time;
	
	gettimeofday(&current_time, NULL);
	return (current_time.tv_usec / 1000 + (current_time.tv_sec * 1000));
}

