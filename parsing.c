/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: filferna <filferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 16:42:32 by filferna          #+#    #+#             */
/*   Updated: 2024/10/03 14:14:59 by filferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_number(char *str)
{
	while(*str)
	{
		if(*str < '0' || *str > '9')
			exit(printf("args is not a positive number\n"));
		str++;
	}
	return (1);
}