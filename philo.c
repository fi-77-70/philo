/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:33:23 by filferna          #+#    #+#             */
/*   Updated: 2024/10/01 21:37:48 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	try_eat(t_philo *philo)
{
	int	eating;

	eating = 1;
	if (philo->id % 2)
		usleep(90);
	while (eating)
	{
		while (pthread_mutex_trylock(&philo->l_fork) != 0)
		{
			printf("philo %d, waiting for fork\n", philo->id);
			pthread_mutex_lock(&philo->l_fork);
		}
		printf("philo %d, has taken a fork\n", philo->id);
		if(pthread_mutex_trylock(&philo->r_fork) != 0)
		{
			printf("philo %d, waiting for fork\n", philo->id);
			pthread_mutex_lock(&philo->r_fork);
		}
		printf("philo %d, has taken a fork\n", philo->id);
		printf("philo %d, is eating\n", philo->id);
		usleep(1000000);
		pthread_mutex_unlock(&philo->l_fork);
		pthread_mutex_unlock(&philo->r_fork);
		philo->meals += 1;
		eating = 0;
		if(philo->meals == 5)
			return(0);
	}
	return (go_to_sleep(philo));	
}

int	go_to_sleep(t_philo *philo)
{
	int	sleeping;

	sleeping = 1;
	while (sleeping)
	{
		printf("philo %d, is sleeping\n", philo->id);
		usleep(150);
		sleeping = 0;
		if(philo->meals == 5)
			return(0);
	}
	return (think(philo));
}

int	think(t_philo *philo)
{
	int	thinking;

	thinking = 1;
	while (thinking)
	{
		printf("philo %d, is thinking\n", philo->id);
		usleep(200);
		thinking = 0;
		if(philo->meals == 5)
			return(0);
	}
	return (try_eat(philo));
}

void	*loop(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	try_eat(philo);
	go_to_sleep(philo);
	think(philo);
	return (NULL);
}

void	init_philos(t_table *table, int seats)
{
	int	i;
	
	table->n_philos = seats;
	i = -1;
	table->philo = malloc(sizeof(t_philo) * seats);
	table->mutex_original = malloc(sizeof(pthread_mutex_t) * seats);
	while(++i < seats)
	{
		table->philo[i].id = i + 1;
		table->philo[i].is_dead = 0;
		table->philo[i].meals = 0;
		table->philo->table = table;
		pthread_mutex_init(&table->mutex_original[i], NULL);
		table->philo->l_fork = table->mutex_original[i];
		if(i > 0)
		{
			table->philo[i - 1].r_fork = table->philo[i].l_fork;
			if(i == (seats - 1))
				table->philo[i].r_fork = table->philo[0].l_fork;
		}
	}
}

void	start_philos(t_table *table, int seats)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i < seats)
		pthread_create(&table->philo[i].th, NULL, &loop, (void *)&table->philo[i]);
	while (++j < seats)
		pthread_join(table->philo[j].th, NULL);
}

int	main(int ac, char **av)
{
	t_table *table;
	int		seats;

	table = (t_table *)malloc(sizeof(t_table));
	if(ac != 2 || !is_number(av[1]))
		return (printf("nao se usa assim tosco"));
	seats = ft_atoi(av[1]);
	printf("ok\n");
	init_philos(table, seats);
	start_philos(table, seats);
	free(table);
}
