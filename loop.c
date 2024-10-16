/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:36:15 by filferna          #+#    #+#             */
/*   Updated: 2024/10/16 14:15:08 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checker(t_philo *philo)
{
	pthread_mutex_lock(*philo->table->checker_mutex);
	if (philo->table->some_one_dead)
		return (pthread_mutex_unlock(*philo->table->checker_mutex), 1);
	return (pthread_mutex_unlock(*philo->table->checker_mutex), 0);
}

int	wait(t_philo *philo, int time_to_wait)
{
	philo->current_time = get_time();
	while (get_time() - philo->current_time < time_to_wait)
	{
		if (checker(philo))
			return (1);
	}
	return (0);
}

int	try_eat(t_philo *philo)
{
	pthread_mutex_t	*fork_1;
	pthread_mutex_t	*fork_2;

	set_forks(philo, &fork_1, &fork_2);
	pthread_mutex_lock(fork_1);
	if(checker(philo))
		return (pthread_mutex_unlock(fork_1), 1);
	printf("%ld philo %d, has taken a fork\n", get_time() - philo->start_time, philo->id);
	pthread_mutex_lock(fork_2);
	if (checker(philo))
		return (pthread_mutex_unlock(fork_1), pthread_mutex_unlock(fork_2), 1);
	printf("%ld philo %d, has taken a fork\n", get_time() - philo->start_time, philo->id);
	pthread_mutex_lock(*philo->table->checker_mutex);
	philo->table->meals += 1;
	philo->last_meal = get_time();
	printf("%ld philo %d, is eating\n", get_time() - philo->start_time, philo->id);
	pthread_mutex_unlock(*philo->table->checker_mutex);
	if (checker(philo))
		return (pthread_mutex_unlock(fork_1), pthread_mutex_unlock(fork_2), 1);
	if (wait(philo, philo->time_to_eat))
		return (pthread_mutex_unlock(fork_1), pthread_mutex_unlock(fork_2), 1);
	pthread_mutex_unlock(fork_1);
	pthread_mutex_unlock(fork_2);
	if (checker(philo))
		return (1);
	return (go_to_sleep(philo));
}

int	go_to_sleep(t_philo *philo)
{
	if (checker(philo))
		return (1);
	printf("%ld philo %d, is sleeping\n", get_time() - philo->start_time, philo->id);
	if (wait(philo, philo->time_to_sleep))
		return (1);
	return (think(philo));
}

int	think(t_philo *philo)
{
	if(checker(philo))
		return (1);
	printf("%ld philo %d, is thinking\n", get_time() - philo->start_time, philo->id);
	return (try_eat(philo));
}

void	*loop(void *arg)
{
	t_philo	*philo;
	int		state;
	
	philo = (t_philo *)arg;
	pthread_mutex_lock(*philo->table->checker_mutex);
	philo->table->ready += 1;
	pthread_mutex_unlock(*philo->table->checker_mutex);
	pthread_mutex_lock(*philo->table->checker_mutex);
	while (philo->table->ready != philo->table->n_philos)
	{
		pthread_mutex_unlock(*philo->table->checker_mutex);
		pthread_mutex_lock(*philo->table->checker_mutex);
	}
	pthread_mutex_unlock(*philo->table->checker_mutex);
	state = try_eat(philo);
	if (state == 1)
		return (NULL);
	return (NULL);
}