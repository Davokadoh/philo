/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:24:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/23 15:29:57 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_ph *ph)
{
	pthread_mutex_lock(&ph->m_last_meal);
	if (is_end(ph))
	{
		pthread_mutex_unlock(&ph->m_last_meal);
		return ;
	}
	ph->last_meal = now(0);
	pthread_mutex_unlock(&ph->m_last_meal);
	printf("%li Philo %i is eating his meal %i.\n", \
			now(0), ph->id + 1, ph->meals_eaten + 1);
	ph->meals_eaten++;
	if (ph->data->max_meal != 0 && ph->meals_eaten >= ph->data->max_meal)
	{
		pthread_mutex_lock(&ph->data->m_end);
		ph->finished = 1;
		ph->data->all_finished++;
		pthread_mutex_unlock(&ph->data->m_end);
	}
	msleep(ph->data->eat_time);
}

void	zzz(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return ;
	}
	printf("%li Philo %i is sleeping.\n", now(0), ph->id + 1);
	pthread_mutex_unlock(&ph->data->m_end);
	msleep(ph->data->zzz_time);
}

void	think(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return ;
	}
	printf("%li Philo %i is thinking.\n", now(0), ph->id + 1);
	pthread_mutex_unlock(&ph->data->m_end);
}
