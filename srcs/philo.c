/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/22 11:30:13 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *msg)
{
	printf("%s\n", msg);
	return (1);
}

int	is_finished(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->finished_mutex);
	if (ph->data->all_finished == ph->data->nbr)
	{
		pthread_mutex_unlock(&ph->data->finished_mutex);
		return (1);
	}
	pthread_mutex_unlock(&ph->data->finished_mutex);
	return (0);
}

int	is_dead(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
	return (0);
}

long	now(int init)
{
    struct timeval		time;
	static long			start = 0;
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
	long			stamp;

	gettimeofday(&time, NULL);
	stamp = time.tv_sec * 1000L + time.tv_usec / 1000;
	start = stamp;
	while (stamp - start < msec)
	{
		usleep(100);
		gettimeofday(&time, NULL);
		stamp = time.tv_sec * 1000L + time.tv_usec / 1000;
	}
}

void	take_fork(t_ph *ph, int frk, pthread_mutex_t *frks)
{
	frk %= ph->data->nbr;
	pthread_mutex_lock(&frks[frk]);
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->dead)
	{
		pthread_mutex_unlock(&ph->data->death_mutex);
		return ;
	}
	printf("%li Philo %i has taken fork %i.\n", now(0), ph->id + 1, frk + 1);
	pthread_mutex_unlock(&ph->data->death_mutex);
}

void	take_forks(t_ph *ph, pthread_mutex_t *frks)
{
	int	id;

	id = ph->id;
	take_fork(ph, id + ((id + 1) % 2), frks);
	take_fork(ph, id + (id % 2), frks);
	ph->has_forks = 1;
}

void	drop_forks(t_ph *ph, pthread_mutex_t *frks)
{
	pthread_mutex_unlock(&frks[ph->id]);
	pthread_mutex_unlock(&frks[(ph->id + 1) % ph->data->nbr]);
	ph->has_forks = 0;
}

void	zzz(int time, int i)
{
	printf("%li Philo %i is sleeping.\n", now(0), i + 1);
	msleep(time);
}

void	think(int i)
{
	printf("%li Philo %i is thinking.\n", now(0), i + 1);
}

void	die(t_ph *ph)
{
	pthread_mutex_lock(&ph->data->death_mutex);
	if (ph->data->dead == 0)
	{
		ph->data->dead = 1;
		printf("%li Philo %i died. Last meal %li msec ago.\n", now(0), ph->id + 1, now(0) - ph->last_meal);
	}
	pthread_mutex_unlock(&ph->data->death_mutex);
}

void	eat(t_ph *ph)
{
	pthread_mutex_lock(&ph->last_meal_mutex);
	ph->last_meal = now(0);
	pthread_mutex_unlock(&ph->last_meal_mutex);
	printf("%li Philo %i is eating his meal %i.\n", now(0), ph->id + 1, ph->meals_eaten);
	msleep(ph->data->eat_time);
	ph->meals_eaten++;
	if (ph->data->max_meal != 0 && ph->meals_eaten >= ph->data->max_meal)
	{
		pthread_mutex_lock(&ph->data->finished_mutex);
		ph->data->all_finished++;
		pthread_mutex_unlock(&ph->data->finished_mutex);
	}
}

void	*routine(void *void_philo)
{
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	if (ph->data->nbr == 1)
	{
		printf("%li Philo 1 has taken fork 1.\n", now(0));
		msleep(ph->data->death_time);
		pthread_exit(NULL);
	}
	if (ph->id % 2)
		usleep(100);
	while (ph->meals_eaten < ph->data->max_meal || ph->data->max_meal == 0)
	{
		take_forks(ph, ph->frks);
		if (is_dead(ph))
			break;
		eat(ph);
		drop_forks(ph, ph->frks);
		if (is_dead(ph))
			break;
		zzz(ph->data->zzz_time, ph->id);
		if (is_dead(ph))
			break;
		think(ph->id);
	}
	if (ph->has_forks)
		drop_forks(ph, ph->frks);
	pthread_join(ph->death_timer, NULL);
	pthread_exit(NULL);
}

void	threads(t_ph *ph, t_data *data, pthread_mutex_t *frks, pthread_t *tids)
{
	int	i;

	i = -1;
	while (++i < data->nbr)
	{
		ph[i].id = i;
		ph[i].data = data;
		ph[i].frks = frks;
		ph[i].meals_eaten = 0;
		ph[i].last_meal = 0;
		pthread_mutex_init(&ph[i].last_meal_mutex, NULL);
		pthread_create(&tids[i], NULL, routine, (void *) &ph[i]);
	}
}

void	wait_destroy_mutexes(int max, pthread_t *tids, pthread_mutex_t *frks)
{
	int	i;

	i = -1;
	while (++i < max)
		pthread_join(tids[i], NULL);
	i = -1;
	while (++i < max)
		pthread_mutex_destroy(&frks[i]);
}

void	free_arrays(t_ph *ph, pthread_t *tids, pthread_mutex_t *frks)
{
	free(tids);
	free(frks);
	free(ph);
}

void	init_forks(int max, pthread_mutex_t *frks)
{
	int	i;

	i = -1;
	while (++i < max)
		pthread_mutex_init(&frks[i], NULL);
}

int	malloc_arrays(int nbr, t_ph **ph, pthread_t **tids, pthread_mutex_t **frks)
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

t_data	get_data(int ac, char *av[])
{
	t_data	data;

	//Check if args are ints before atoiing
	data.nbr = ft_atoi(av[1]);
	data.death_time = ft_atoi(av[2]);
	data.eat_time = ft_atoi(av[3]);
	data.zzz_time = ft_atoi(av[4]);
	if (ac == 6)
		data.max_meal = ft_atoi(av[5]);
	else
		data.max_meal = 0;
	data.dead = 0;
	data.all_finished = 0;
	pthread_mutex_init(&data.death_mutex, NULL);
	pthread_mutex_init(&data.finished_mutex, NULL);
	return (data);
}

void	death_check(t_ph *ph, int max)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < max)
		{
			if (is_dead(&ph[0]) || is_finished(&ph[0]))
				break ;
			pthread_mutex_lock(&ph[i].last_meal_mutex);
			if (now(0) - ph[i].last_meal > ph[i].data->death_time)
				die(ph);
			pthread_mutex_unlock(&ph[i].last_meal_mutex);
		}
		if (is_dead(&ph[0]) || is_finished(&ph[0]))
			break ;
	}
}

int	main(int ac, char *av[])
{
	t_data			data;
	t_ph			*ph;
	pthread_t		*tids;
	pthread_mutex_t	*frks;

	if (ac != 5 && ac != 6)
		return (ft_error("Wrong number of arguments."));
	data = get_data(ac, av);
	if (malloc_arrays(data.nbr, &ph, &tids, &frks))
		return (ft_error("Malloc fail."));
	init_forks(data.nbr, frks);
	now(1);
	threads(ph, &data, frks, tids); //Check if tids created ?
	death_check(ph, data.nbr);
	wait_destroy_mutexes(data.nbr, tids, frks);
	printf("Finished : %i/%i\n", ph[0].data->all_finished, ph[0].data->nbr);
	free_arrays(ph, tids, frks);
	return (0);
}
