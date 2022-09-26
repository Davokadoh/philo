/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/26 13:05:52 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*routine(void *void_philo)
{
	t_ph		*ph;

	ph = (t_ph *) void_philo;
	if (ph->id % 2)
		usleep(100);
	while (ph->meals_eaten < ph->data->max_meal || ph->data->max_meal == 0)
	{
		take_forks(ph, ph->frks);
		eat(ph);
		drop_forks(ph, ph->frks);
		if (is_end(ph))
			break ;
		zzz(ph);
		if (is_end(ph))
			break ;
		think(ph);
	}
	if (ph->has_forks)
		drop_forks(ph, ph->frks);
	pthread_exit(NULL);
}

static void	init(t_ph *ph, t_data *data, pthread_mutex_t *frks, pthread_t *tids)
{
	int	i;

	pthread_mutex_init(&data->m_write, NULL);
	i = -1;
	while (++i < data->nbr)
		pthread_mutex_init(&frks[i], NULL);
	now(1);
	i = -1;
	while (++i < data->nbr)
	{
		ph[i].id = i;
		ph[i].data = data;
		ph[i].frks = frks;
		ph[i].meals_eaten = 0;
		ph[i].finished = 0;
		ph[i].last_meal = 0;
		pthread_mutex_init(&ph[i].m_last_meal, NULL);
		pthread_create(&tids[i], NULL, routine, (void *) &ph[i]);
	}
}

static void	destroy_mutexes(int max, pthread_t *tids, pthread_mutex_t *frks)
{
	int	i;

	i = -1;
	while (++i < max)
		pthread_join(tids[i], NULL);
	i = -1;
	while (++i < max)
		pthread_mutex_destroy(&frks[i]);
}

static t_data	get_data(int ac, char *av[])
{
	t_data	data;

	data.nbr = ft_atoi(&av[1]);
	if (data.nbr < 1)
		exit(ft_error("Need a number of Philosopher > 0", 2));
	data.death_time = ft_atoi(&av[2]);
	data.eat_time = ft_atoi(&av[3]);
	data.zzz_time = ft_atoi(&av[4]);
	if (ac == 6)
		data.max_meal = ft_atoi(&av[5]);
	else
		data.max_meal = 0;
	data.dead = 0;
	data.all_finished = 0;
	pthread_mutex_init(&data.m_end, NULL);
	return (data);
}

int	main(int ac, char *av[])
{
	t_data			data;
	t_ph			*ph;
	pthread_t		*tids;
	pthread_mutex_t	*frks;

	if (ac != 5 && ac != 6)
		return (ft_error("Wrong number of arguments.", 1));
	data = get_data(ac, av);
	if (malloc_arr(data.nbr, &ph, &tids, &frks))
		return (ft_error("Malloc fail.", 3));
	init(ph, &data, frks, tids);
	death_check(ph, data.nbr);
	destroy_mutexes(data.nbr, tids, frks);
	pthread_mutex_destroy(&data.m_write);
	pthread_mutex_destroy(&data.m_end);
	printf("Finished : %i/%i\n", ph[0].data->all_finished, ph[0].data->nbr);
	free_arrays(ph, tids, frks);
	return (0);
}
