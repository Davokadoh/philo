/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/14 14:39:19 by jleroux          ###   ########.fr       */
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
	milliseconds = time.tv_sec*1000LL + time.tv_usec/1000;
	if (init)
		start = milliseconds;
	return (milliseconds - start);
}

void	msleep(int msec)
{
	int	start;

	start = now(0);
	while (now(0) - start < msec)
		usleep(100);
}

void	take_fork(t_ph *ph, int frk, pthread_mutex_t *frks)
{
	if (frk >= ph->data->nbr)
		frk = 0;
	pthread_mutex_lock(&frks[frk]);
	printf("%lli Philo %i has taken fork %i.\n", now(0), ph->id + 1, frk + 1);
}

void	take_forks(t_ph *ph, pthread_mutex_t *frks)
{
	if (ph->id % 2 == 0)
	{
		take_fork(ph, ph->id + 1, frks);
		take_fork(ph, ph->id, frks);
	}
	else
	{
		take_fork(ph, ph->id, frks);
		take_fork(ph, ph->id + 1, frks);
	}
}

void	drop_forks(t_ph *ph, pthread_mutex_t *frks)
{
	pthread_mutex_unlock(&frks[ph->id]);
	if (ph->id + 1 >= ph->data->nbr)
		pthread_mutex_unlock(&frks[0]);
	else
		pthread_mutex_unlock(&frks[ph->id + 1]);
}

void	eat(t_ph *ph)
{
	printf("%lli Philo %i is eating.\n", now(0), ph->id + 1);
	msleep(ph->data->eat_time);
	ph->last_meal = now(0);
	ph->meals_eaten++;
}

void	zzz(int time, int i)
{
	printf("%lli Philo %i is sleeping.\n", now(0), i + 1);
	msleep(time);
	printf("%lli Philo %i is thinking.\n", now(0), i + 1);
}

void	die(t_ph *ph)
{
	ph->data->dead = 1;
	printf("%lli Philo %i died.\n", now(0), ph->id);
}

void	*death_timer(void *void_philo)
{
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	pthread_detach(pthread_self());
	while (1)
	{
		msleep(ph->data->death_time);
		//printf("Philo %i last meal was %lli ms ago.\n" , ph->id + 1, now(0) - ph->last_meal);
		if (now(0) - ph->last_meal >= ph->data->death_time)
		{
			die(ph);
			break;
		}
	}
	pthread_exit(NULL);
}

void	*routine(void *void_philo)
{
	t_ph		*ph;
	pthread_t	death_thread;

	ph = (t_ph *) void_philo;
	ph->last_meal = 0;
	pthread_create(&death_thread, NULL, death_timer, (void *) ph);
	while (ph->meals_eaten < ph->data->max_meal || ph->data->max_meal == 0)
	{
		if (ph->data->dead)
			break;
		take_forks(ph, ph->frks);
		if (ph->data->dead)
			break;
		eat(ph);
		if (ph->data->dead)
			break;
		drop_forks(ph, ph->frks);
		if (ph->data->dead)
			break;
		zzz(ph->data->zzz_time, ph->id);
	}
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
	*tids = malloc(nbr * sizeof(pthread_t));
	*frks = malloc(nbr * sizeof(pthread_mutex_t));
	if (!*ph || !*tids || !*frks)
		return (1);
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
	data.start_time = now(1);
	threads(ph, &data, frks, tids); //Check if tids created ?
	wait_destroy_mutexes(data.nbr, tids, frks);
	free_arrays(ph, tids, frks); //Check if arrays exist before freeing
	return (0);
}
