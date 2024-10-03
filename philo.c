/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:33:23 by filferna          #+#    #+#             */
/*   Updated: 2024/10/03 19:40:47 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

suseconds_t	get_time(void)
{
	struct timeval current_time;
	
	gettimeofday(&current_time, NULL);
	return (current_time.tv_usec / 1000 + (current_time.tv_sec * 1000));
}

int	checker(t_philo *philo)
{
	pthread_mutex_lock(philo->table->checker_mutex[0]);
	if (philo->table->some_one_dead)
		return (pthread_mutex_unlock(philo->table->checker_mutex[0]), 1);
	if (philo->meals == philo->table->max_meals && philo->table->max_meals != -1)
		return (pthread_mutex_unlock(philo->table->checker_mutex[0]), 1);
	if ((get_time() - philo->last_meal) > philo->time_to_die)
		return (printf("%ld philo %d, is dead\n",get_time() - philo->table->start_time, philo->id), philo->table->some_one_dead = 1, pthread_mutex_unlock(philo->table->checker_mutex[0]), 1);
	pthread_mutex_unlock(philo->table->checker_mutex[0]);
	return (0);
	
}
int	wait(t_philo *philo, int time_to_wait)
{
	philo->current_time = get_time();
	while (get_time() - philo->current_time < time_to_wait)
	{
		if (checker(philo) == 1)
			return (1);
	}
	return (0);
}

int	try_eat(t_philo *philo)
{
	pthread_mutex_t *fork_1;
	pthread_mutex_t *fork_2;

	if (philo->id % 2)
	{
		fork_1 = philo->l_fork;
		fork_2 = philo->r_fork;
	}
	else
	{
		fork_2 = philo->l_fork;
		fork_1 = philo->r_fork;
	}
	if (checker(philo))
		return (1);
	pthread_mutex_lock(fork_1);
	if(checker(philo))
		return (pthread_mutex_unlock(fork_1), 1);
	printf("%ld philo %d, has taken a fork\n", get_time() - philo->table->start_time, philo->id);
	pthread_mutex_lock(fork_2);
	if(checker(philo))
		return (pthread_mutex_unlock(fork_1), pthread_mutex_unlock(fork_2), 1);
	printf("%ld philo %d, has taken a fork\n",get_time() - philo->table->start_time, philo->id);
	philo->last_meal = get_time();
	printf("%ld philo %d, is eating\n", get_time() - philo->table->start_time, philo->id);
	if(wait(philo, philo->time_to_eat))
		return (pthread_mutex_unlock(fork_1), pthread_mutex_unlock(fork_2), 1);
	pthread_mutex_unlock(fork_1);
	pthread_mutex_unlock(fork_2);
	philo->meals += 1;
	return (go_to_sleep(philo));	
}

int	go_to_sleep(t_philo *philo)
{
	if (checker(philo))
		return (1);
	printf("%ld philo %d, is sleeping\n", get_time() - philo->table->start_time, philo->id);
	philo->current_time = get_time();
	if (wait(philo, philo->time_to_sleep))
		return (1);
	return (think(philo));
}

int	think(t_philo *philo)
{
	if(checker(philo))
		return (1);
	printf("%ld philo %d, is thinking\n", get_time() - philo->table->start_time, philo->id);
	return (try_eat(philo));
}

void	*loop(void *arg)
{
	t_philo	*philo;
	int		state;
	
	philo = (t_philo *)arg;
	state = try_eat(philo);
	if (state == 1)
		return (NULL);
	return (NULL);
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
		table->philo[i].meals = 0;
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

	i = -1;
	j = -1;
	table->start_time = get_time();
	while (++i < table->n_philos)
	{
		table->philo[i].last_meal = get_time();
		pthread_create(&table->philo[i].th, NULL, &loop, (void *)&table->philo[i]);
	}
	while (++j < table->n_philos)
		pthread_join(table->philo[j].th, NULL);
}

void	free_table(t_table *table)
{
	free(table);
}

void	init_table(t_table *table, char **av)
{
	table->n_philos = ft_atoi(av[1]);
	table->some_one_dead = 0;
	table->checker_mutex = malloc(sizeof(pthread_mutex_t *) * 1);
	table->checker_mutex[0] = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(table->checker_mutex[0], NULL);
	if (av[5])
		table->max_meals = ft_atoi(av[5]);
	else
		table->max_meals = -1;
}

int	main(int ac, char **av)
{
	t_table *table;
	int		i;

	if(ac < 4 || ac > 6)
		return(printf("Error, args(n philos, t_die, t_eat, t_sleep)\n"), 0);
	i = 0;
	while (av[++i])
		is_number(av[i]);
	if (ft_atoi(av[1]) == 1)
	{	
		printf("1 philo 1 has taken a fork\n");
		usleep(ft_atoi(av[2]) * 1000);
		return(printf("%d philo 1, is dead\n", ft_atoi(av[2])));
	}
	table = (t_table *)malloc(sizeof(t_table));
	init_table(table, av);
	init_philos(table, av);
	start_philos(table);
	i = -1;
	while (++i < table->n_philos)
		pthread_mutex_destroy(&table->mutex_original[i]);
	pthread_mutex_destroy(table->checker_mutex[0]);
	free(table->checker_mutex[0]);
	free(table->checker_mutex);
	free(table->philo);
	free(table->mutex_original);
	free(table);
}
