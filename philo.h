/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:33:49 by filferna          #+#    #+#             */
/*   Updated: 2024/10/03 16:44:12 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>

typedef	struct s_table t_table;
typedef	struct s_philo t_philo;

typedef	struct s_philo{
	pthread_t 		th;
	pthread_mutex_t *l_fork;
	pthread_mutex_t	*r_fork;
	t_table			*table;
	int				is_dead;
	int				meals;
	suseconds_t		current_time;
	suseconds_t		last_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				id;
} t_philo;

typedef	struct s_table{
	t_philo			*philo;
	pthread_mutex_t	*mutex_original;
	pthread_mutex_t	*checker_mutex;
	suseconds_t		start_time;
	int				n_philos;
	int				max_meals;
	int				some_one_dead;
} t_table;

int		ft_atoi(char *str);
int		is_number(char *str);
void	init_philos(t_table *table, char **av);
int		think(t_philo *philo);
int		go_to_sleep(t_philo *philo);

#endif