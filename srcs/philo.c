/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/13 10:56:39 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_error(char *msg)
{
	printf("%s\n", msg);
	return (1);
}

void	take_fork(t_ph ph, int frk, pthread_mutex_t *frks)
{
	if (frk >= ph.rules.philo_nbr)
		frk = 0;
	pthread_mutex_lock(&frks[frk]);
	printf("Philo %i has taken fork %i\n", ph.id + 1, frk + 1);
}

void	eat(t_ph *ph, pthread_mutex_t *frks)
{
	if (ph->id % 2 == 0)
	{
		take_fork(*ph, ph->id + 1, frks);
		take_fork(*ph, ph->id, frks);
	}
	else
	{
		take_fork(*ph, ph->id, frks);
		take_fork(*ph, ph->id + 1, frks);
	}
	printf("Philo %i is eating\n", ph->id + 1);
	usleep(ph->rules.eat_time);
	ph->meals_eaten++;
	pthread_mutex_unlock(&frks[ph->id]);
	if (ph->id + 1 >= ph->rules.philo_nbr)
		pthread_mutex_unlock(&frks[0]);
	else
		pthread_mutex_unlock(&frks[ph->id + 1]);
}

void	zzz(int time, int i)
{
	(void)i;
	printf("Philo %i is sleeping\n", i + 1);
	usleep(time);
	printf("Philo %i is thinking\n", i + 1);
}

void	*routine(void *void_philo)
{
	t_ph	*ph;

	ph = (t_ph *) void_philo;
	while (ph->meals_eaten < ph->rules.max_meal || ph->rules.max_meal == 0)
	{
		eat(ph, ph->frks);
		zzz(ph->rules.sleep_time, ph->id);
	}
	pthread_exit(NULL);
}

void	threads(t_ph *ph, t_rules rules, pthread_mutex_t *frks, pthread_t *tids)
{
	int	i;

	printf("test\n");
	i = -1;
	while (++i < rules.philo_nbr)
	{
		ph[i].id = i;
		ph[i].rules = rules;
		ph[i].frks = frks;
		ph[i].meals_eaten = 0;
		pthread_create(&tids[i], NULL, routine, (void *) &ph[i]);
	}
}

void	wait_then_dtry_mutexes(int max, pthread_t *tids, pthread_mutex_t *frks)
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

t_rules	get_rules(int ac, char *av[])
{
	t_rules	rules;

	rules.philo_nbr = ft_atoi(av[1]);
	rules.death_time = ft_atoi(av[2]);
	rules.eat_time = ft_atoi(av[3]);
	rules.sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		rules.max_meal = ft_atoi(av[5]);
	else
		rules.max_meal = 0;
	return (rules);
}

int	main(int ac, char *av[])
{
	t_rules			rules;
	t_ph			*ph;
	pthread_t		*tids;
	pthread_mutex_t	*frks;

	if (ac != 5 && ac != 6)
		return (ft_error("Wrong number of arguments"));
	rules = get_rules(ac, av); //Check if args are all ints
	if (malloc_arrays(rules.philo_nbr, &ph, &tids, &frks))
		return (ft_error("malloc fail"));
	init_mutexes(rules.philo_nbr, frks); //Protect mutexes ?
	threads(ph, rules, frks, tids); //Check if tids created ?
	wait_then_dtry_mutexes(rules.philo_nbr, tids, frks); //Nothing to be done ?
	free_arrays(ph, tids, frks); //Check if arrays exist before freeing
	return (0);
}
