/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/19 12:51:58 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *msg)
{
	printf("%s\n", msg);
	return (1);
}

long long	now(int init)
{
    struct timeval		time;
	static long long	start = 0;
	long long			milliseconds;

	gettimeofday(&time, NULL);
	milliseconds = time.tv_sec * 1000LL + time.tv_usec / 1000;
	if (init)
		start = milliseconds;
	return (milliseconds - start);
}

void	msleep(long long msec)
{
	//usleep(msec * 1000);
    struct timeval	time;
	long long		start;
	long long		stamp;

	gettimeofday(&time, NULL);
	stamp = time.tv_sec * 1000LL + time.tv_usec / 1000;
	start = stamp;
	while (stamp - start < msec)
	{
		usleep(100);
		gettimeofday(&time, NULL);
		stamp = time.tv_sec * 1000LL + time.tv_usec / 1000;
	}
}

void	take_fork(t_ph *ph, int frk, pthread_mutex_t *frks)
{
	frk %= ph->data->nbr;
	pthread_mutex_lock(&frks[frk]);
	if (ph->data->dead)
		return;
	printf("%lli Philo %i has taken fork %i.\n", now(0), ph->id + 1, frk + 1);
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

void	eat(t_ph *ph)
{
	ph->meals_eaten++;
	if (ph->meals_eaten >= ph->data->max_meal)
		ph->data->finished++;
	ph->last_meal = now(0);
	printf("%lli Philo %i is eating his meal %i.\n", now(0), ph->id + 1, ph->meals_eaten);
	msleep(ph->data->eat_time);
}

void	zzz(int time, int i)
{
	printf("%lli Philo %i is sleeping.\n", now(0), i + 1);
	msleep(time);
}

void	think(int i)
{
	printf("%lli Philo %i is thinking.\n", now(0), i + 1);
}

void	die(t_ph *ph)
{
	ph->data->dead = 1;
	printf("%lli Philo %i died. Last meal (%i/%i) %lli msec ago.\n", now(0), ph->id + 1, ph->meals_eaten, ph->data->max_meal, now(0) - ph->last_meal);
}

void	*death_routine(void *void_philo)
{
	int			i;
	int			max;
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	max = ph[0].data->nbr;
	while (1)
	{
		i = -1;
		while (++i < max)
		{
			if (ph[0].data->dead || ph[0].data->finished == max)
				break ;
			if (ph[i].meals_eaten < ph[i].data->max_meal && !ph[0].data->dead &&
					now(0) - ph[i].last_meal >= ph[i].data->death_time)
				die(&ph[i]);
		}
		if (ph[0].data->dead || ph[0].data->finished == max)
			break ;
	}
	pthread_exit(NULL);
}

void	*routine(void *void_philo)
{
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	if (ph->data->nbr == 1)
	{
		printf("%lli Philo %i has taken fork %i.\n", now(0), ph->id + 1, 1);
		msleep(ph->data->death_time);
		//ph->data->dead = 1;
		pthread_exit(NULL);
	}
	if (ph->id % 2)
		usleep(150);
	while (ph->meals_eaten < ph->data->max_meal || ph->data->max_meal == 0)
	{
		take_forks(ph, ph->frks);
		if (ph->data->dead)
			break;
		eat(ph);
		drop_forks(ph, ph->frks);
		if (ph->data->dead)
			break;
		zzz(ph->data->zzz_time, ph->id);
		if (ph->data->dead)
			break;
		think(ph->id);
	}
	if (ph->has_forks)
		drop_forks(ph, ph->frks);
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

void	init_mutexes(int max, pthread_mutex_t *frks)
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
	*tids = malloc((nbr + 1) * sizeof(pthread_t));
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

t_data	get_rules(int ac, char *av[])
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
	data.finished = 0;
	return (data);
}

int	main(int ac, char *av[])
{
	t_data			data;
	t_ph			*ph;
	pthread_t		*tids;
	pthread_mutex_t	*frks;

	if (ac != 5 && ac != 6)
		return (ft_error("Wrong number of arguments."));
	data = get_rules(ac, av);
	if (malloc_arrays(data.nbr, &ph, &tids, &frks))
		return (ft_error("Malloc fail."));
	init_mutexes(data.nbr, frks); //Protect mutexes ?
	now(1);
	threads(ph, &data, frks, tids); //Check if tids created ?
	pthread_create(&tids[data.nbr], NULL, death_routine, (void *) ph);
	wait_destroy_mutexes(data.nbr, tids, frks);
	pthread_join(tids[data.nbr], NULL);
	printf("Finished : %i/%i\n", ph[0].data->finished, ph[0].data->nbr);
	free_arrays(ph, tids, frks);
	return (0);
}
