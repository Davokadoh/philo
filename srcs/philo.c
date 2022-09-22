/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/22 14:33:57 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "ft_printf.h"

int	ft_error(char *msg, int errno)
{
	printf("%s\n", msg);
	return (errno);
}

int	is_end(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->all_finished == ph->data->nbr || ph->data->dead)
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

long	now(int init)
{
    struct timeval		time;
	static long			start;
	long				milliseconds;

	gettimeofday(&time, NULL);
	milliseconds = time.tv_sec * 1000L + time.tv_usec / 1000;
	if (init)
		start = milliseconds;
	return (milliseconds - start);
}

void	msleep(long long msec)
{
    struct timeval	time;
	long			start;
	long			now;

	gettimeofday(&time, NULL);
	now = time.tv_sec * 1000L + time.tv_usec / 1000;
	start = now;
	while (now - start < msec)
	{
		usleep(100);
		gettimeofday(&time, NULL);
		now = time.tv_sec * 1000L + time.tv_usec / 1000;
	}
}

void	take_fork(t_ph *ph, int frk, pthread_mutex_t *m_frks)
{
	frk %= ph->data->nbr;
	pthread_mutex_lock(&m_frks[frk]);
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->m_end);
		return ;
	}
	m_printf(&ph->data->m_write, "%i Philo %i has taken fork %i.\n", now(0), ph->id + 1, frk + 1);
	pthread_mutex_unlock(&ph->data->m_end);
}

void	take_forks(t_ph *ph, pthread_mutex_t *m_frks)
{
	int	id;

	id = ph->id;
	take_fork(ph, id + ((id + 1) % 2), m_frks);
	take_fork(ph, id + (id % 2), m_frks);
	ph->has_forks = 1;
}

void	drop_forks(t_ph *ph, pthread_mutex_t *m_frks)
{
	pthread_mutex_unlock(&m_frks[ph->id]);
	pthread_mutex_unlock(&m_frks[(ph->id + 1) % ph->data->nbr]);
	ph->has_forks = 0;
}

void	zzz(t_ph *ph)
{
	m_printf(&ph->data->m_write, "%i Philo %i is sleeping.\n", now(0), ph->id + 1);
	msleep(ph->data->zzz_time);
}

void	think(t_ph *ph)
{
	m_printf(&ph->data->m_write, "%i Philo %i is thinking.\n", now(0), ph->id + 1);
}

void	die(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->m_end);
	if (ph->data->dead == 0)
	{
		ph->data->dead = 1;
		m_printf(&ph->data->m_write, "%i Philo %i died. Last meal %i msec ago.\n", now(0), ph->id + 1, now(0) - ph->last_meal);
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
	m_printf(&ph->data->m_write, "%i Philo %i is eating his meal %i.\n", now(0), ph->id + 1, ph->meals_eaten + 1);
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

void	*routine(void *void_philo)
{
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	if (ph->data->nbr == 1)
	{
		m_printf(&ph->data->m_write, "%i Philo 1 has taken fork 1.\n", now(0));
		msleep(ph->data->death_time + 2);
		pthread_exit(NULL);
	}
	if (ph->id % 2)
		usleep(100);
	while (ph->meals_eaten < ph->data->max_meal || ph->data->max_meal == 0)
	{
		take_forks(ph, ph->m_frks);
		eat(ph);
		drop_forks(ph, ph->m_frks);
		if (is_end(ph))
			break;
		zzz(ph);
		if (is_end(ph))
			break;
		think(ph);
	}
	if (ph->has_forks)
		drop_forks(ph, ph->m_frks);
	pthread_join(ph->death_timer, NULL);
	pthread_exit(NULL);
}

void	threads(t_ph *ph, t_data *data, pthread_mutex_t *m_frks, pthread_t *tids)
{
	int	i;

	i = -1;
	while (++i < data->nbr)
	{
		ph[i].id = i;
		ph[i].data = data;
		ph[i].m_frks = m_frks;
		ph[i].meals_eaten = 0;
		ph[i].finished = 0;
		ph[i].last_meal = 0;
		pthread_mutex_init(&ph[i].m_last_meal, NULL);
		pthread_create(&tids[i], NULL, routine, (void *) &ph[i]);
	}
}

void	wait_destroy_mutexes(int max, pthread_t *tids, pthread_mutex_t *m_frks)
{
	int	i;

	i = -1;
	while (++i < max)
		pthread_join(tids[i], NULL);
	i = -1;
	while (++i < max)
		pthread_mutex_destroy(&m_frks[i]);
}

void	free_arrays(t_ph *ph, pthread_t *tids, pthread_mutex_t *m_frks)
{
	free(tids);
	free(m_frks);
	free(ph);
}

void	init_forks(int max, pthread_mutex_t *m_frks)
{
	int	i;

	i = -1;
	while (++i < max)
		pthread_mutex_init(&m_frks[i], NULL);
}

int	malloc_arrays(int nbr, t_ph **ph, pthread_t **tids, pthread_mutex_t **m_frks)
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
	*m_frks = malloc(nbr * sizeof(pthread_mutex_t));
	if (!*m_frks)
	{
		free(ph);
		free(tids);
		return (1);
	}
	return (0);
}

t_data	get_data(int ac, char *av[])
{
	t_data	data;

	//Check if args are ints before atoiing
	data.nbr = ft_atoi(&av[1]);
	data.death_time = ft_atoi(&av[2]);
	data.eat_time = ft_atoi(&av[3]);
	data.zzz_time = ft_atoi(&av[4]);
	if (ac == 6)
		data.max_meal = ft_atoi(&av[5]);
	else
		data.max_meal = 0;
	data.dead = 0;
	data.all_finished = 0;
	pthread_mutex_init(&data.m_write, NULL);
	pthread_mutex_init(&data.m_end, NULL);
	return (data);
}

int	main(int ac, char *av[])
{
	t_data			data;
	t_ph			*ph;
	pthread_t		*tids;
	pthread_mutex_t	*m_frks;

	if (ac != 5 && ac != 6)
		return (ft_error("Wrong number of arguments.", 1));
	data = get_data(ac, av);
	if (malloc_arrays(data.nbr, &ph, &tids, &m_frks))
		return (ft_error("Malloc fail.", 2));
	init_forks(data.nbr, m_frks);
	now(1);
	threads(ph, &data, m_frks, tids); //Check if tids created ?
	death_check(ph, data.nbr);
	wait_destroy_mutexes(data.nbr, tids, m_frks);
	printf("Finished : %i/%i\n", ph[0].data->all_finished, ph[0].data->nbr);
	free_arrays(ph, tids, m_frks);
	return (0);
}
