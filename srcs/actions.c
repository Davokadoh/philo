/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:24:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/26 12:58:22 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	m_print(t_ph *ph, char *msg)
{
	pthread_mutex_lock(&ph->data->m_write);
	printf("%li %i %s\n", now(0), ph->id + 1, msg);
	pthread_mutex_unlock(&ph->data->m_write);
}

void	die(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead == 0)
	{
		ph->data->dead = 1;
		m_print(ph, "died.");
	}
	pthread_mutex_unlock(&ph->data->m_end);
}

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
	m_print(ph, "is eating.");
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
	m_print(ph, "is sleeping.");
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
	m_print(ph, "is thinking.");
	pthread_mutex_unlock(&ph->data->m_end);
}
