/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:30:42 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/23 15:36:53 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_fork(t_ph *ph, int frk, pthread_mutex_t *frks)
{
	frk %= ph->data->nbr;
	pthread_mutex_lock(&frks[frk]);
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return ;
	}
	printf("%li Philo %i has taken fork %i.\n", now(0), ph->id + 1, frk + 1);
	pthread_mutex_unlock(&ph->data->m_end);
}

void	take_forks(t_ph *ph, pthread_mutex_t *frks)
{
	int	id;

	id = ph->id;
	take_fork(ph, id + ((id + 1) % 2), frks);
	if (ph->data->nbr == 1)
	{
		msleep(ph->data->death_time + 2);
		return ;
	}
	take_fork(ph, id + (id % 2), frks);
	ph->has_forks = 1;
}

void	drop_forks(t_ph *ph, pthread_mutex_t *frks)
{
	pthread_mutex_unlock(&frks[ph->id]);
	pthread_mutex_unlock(&frks[(ph->id + 1) % ph->data->nbr]);
	ph->has_forks = 0;
}
