/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:33:23 by filferna          #+#    #+#             */
/*   Updated: 2024/10/23 09:10:18 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_everything(t_table *table)
{
	free(table->checker_mutex[0]);
	free(table->checker_mutex);
	free(table->philo);
	free(table->mutex_original);
	free(table);
}

int	main(int ac, char **av)
{
	t_table	*table;
	int		i;

	if (ac < 5 || ac > 6 || ft_atoi(av[2]) == 0)
		return (printf("Error, args(n philos, t_die, t_eat, t_sleep)\n"), 0);
	i = 0;
	while (av[++i])
		is_number(av[i]);
	if (ft_atoi(av[1]) == 1)
	{
		printf("1 philo 1 has taken a fork\n");
		usleep(ft_atoi(av[2]) * 1000);
		return (printf("%d philo 1, is dead\n", ft_atoi(av[2])));
	}
	table = (t_table *)malloc(sizeof(t_table));
	init_table(table, av);
	init_philos(table, av);
	start_philos(table);
	i = -1;
	while (++i < table->n_philos)
		pthread_mutex_destroy(&table->mutex_original[i]);
	pthread_mutex_destroy(table->checker_mutex[0]);
}
