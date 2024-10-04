/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:42:32 by filferna          #+#    #+#             */
/*   Updated: 2024/10/04 16:50:29 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_number(char *str)
{
	int	i;
	
	i = 0;
	while(*str)
	{
		if(*str < '0' || *str > '9')
			exit(printf("args is not a positive number\n"));
		str++;
		i++;
		if (i > 10)
			exit(printf("Error, number too big\n"));
	}
	return (1);
}

void	set_forks(t_philo *philo, pthread_mutex_t **fork_1, pthread_mutex_t **fork_2)
{
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