/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:34:10 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/23 15:37:30 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	die(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead == 0)
	{
		ph->data->dead = 1;
		printf("%li Philo %i died. Last meal %li msec ago.\n", \
				now(0), ph->id + 1, now(0) - ph->last_meal);
	}
	pthread_mutex_unlock(&ph->data->m_end);
}

void	death_check(t_ph *ph, int max)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < max)
		{
			if (is_end(&ph[0]))
				break ;
			if (is_finished(&ph[i]))
				continue ;
			pthread_mutex_lock(&ph[i].m_last_meal);
			if (now(0) - ph[i].last_meal > ph[i].data->death_time)
				die(&ph[i]);
			pthread_mutex_unlock(&ph[i].m_last_meal);
		}
		if (is_end(&ph[0]))
			break ;
	}
}

int	is_end(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->all_finished >= ph->data->nbr || ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return (1);
	}
	pthread_mutex_unlock(&ph->data->m_end);
	return (0);
}

int	is_finished(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->finished)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return (1);
	}
	pthread_mutex_unlock(&ph->data->m_end);
	return (0);
}
