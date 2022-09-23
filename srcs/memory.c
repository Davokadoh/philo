/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:40:08 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/23 15:45:03 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *msg, int errno)
{
	printf("%s\n", msg);
	return (errno);
}

void	free_arrays(t_ph *ph, pthread_t *tids, pthread_mutex_t *frks)
{
	free(tids);
	free(frks);
	free(ph);
}

int	malloc_arrays(int nb, t_ph **ph, pthread_t **tids, pthread_mutex_t **frks)
{
	*ph = malloc(nbr * sizeof(t_ph));
	if (!*ph)
		return (1);
	*tids = malloc(nbr * sizeof(pthread_t));
	if (!*tids)
	{
		free(ph);
		return (1);
	}
	*frks = malloc(nbr * sizeof(pthread_mutex_t));
	if (!*frks)
	{
		free(ph);
		free(tids);
		return (1);
	}
	return (0);
}
