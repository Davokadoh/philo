/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:40:08 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/27 11:25:00 by jleroux          ###   ########.fr       */
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

int	malloc_arr(int nb, t_ph **ph, pthread_t **tids, pthread_mutex_t **frks)
{
	*ph = malloc(nb * sizeof(t_ph));
	if (!*ph)
		return (1);
	*tids = malloc(nb * sizeof(pthread_t));
	if (!*tids)
	{
		free(ph);
		return (1);
	}
	*frks = malloc(nb * sizeof(pthread_mutex_t));
	if (!*frks)
	{
		free(ph);
		free(tids);
		return (1);
	}
	return (0);
}

void	destroy_mutexes(t_data *data, pthread_t *tids, pthread_mutex_t *frks)
{
	int	i;

	pthread_mutex_destroy(&data->m_write);
	pthread_mutex_destroy(&data->m_end);
	i = -1;
	while (++i < data->max_meal)
		pthread_join(tids[i], NULL);
	i = -1;
	while (++i < data->max_meal)
		pthread_mutex_destroy(&frks[i]);
}
