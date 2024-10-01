/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:33:49 by filferna          #+#    #+#             */
/*   Updated: 2024/10/01 21:07:36 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

typedef	struct s_table t_table;
typedef	struct s_philo t_philo;

typedef	struct s_philo{
	pthread_t 		th;
	pthread_mutex_t l_fork;
	pthread_mutex_t	r_fork;
	t_table			*table;
	int				is_dead;
	int				meals;
	int				id;
} t_philo;

typedef	struct s_table{
	t_philo			*philo;
	pthread_mutex_t	*mutex_original;
	int				n_philos;
} t_table;

int		ft_atoi(char *str);
int		is_number(char *str);
void	init_philos(t_table *table, int seats);
int		think(t_philo *philo);
int		go_to_sleep(t_philo *philo);

#endif