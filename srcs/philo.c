/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/12 17:01:15 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	EATING 0
#define	SLEEPING 1

typedef struct	s_rules
{
	int	philo_nbr;
	int	death_time;
	int	eat_time;
	int	sleep_time;
	int	meal_nbr;
}				t_rules;

typedef struct	s_philo
{
	int				i;
	int				state;
	int				meals_eaten;
	t_rules			rules;
	pthread_mutex_t	*forks;
}				t_philo;

void	take_fork(t_philo philo, int frk, pthread_mutex_t *forks)
{
	if (frk >= philo.rules.philo_nbr)
		frk = 0;
	//printf("Philo %i tries to take fork %i\n", philo.i, frk);
	pthread_mutex_lock(&forks[frk]);
	printf("Philo %i has taken fork %i\n", philo.i, frk);
}

void	eat(t_philo *philo, pthread_mutex_t *forks)
{
	//if (philo->i == philo->rules.philo_nbr - 1)
	if (philo->i % 2 == 0)
	{
		take_fork(*philo, philo->i + 1, forks);
		take_fork(*philo, philo->i, forks);
	}
	else
	{
		take_fork(*philo, philo->i, forks);
		take_fork(*philo, philo->i + 1, forks);
	}
	printf("Philo %i is eating\n", philo->i);
	usleep(philo->rules.eat_time);
	pthread_mutex_unlock(&forks[philo->i]);
	if (philo->i + 1 >= philo->rules.philo_nbr)
		pthread_mutex_unlock(&forks[0]);
	else
		pthread_mutex_unlock(&forks[philo->i + 1]);
}

void	zzz(int time, int i)
{
	(void)i;
	//printf("Philo %i is sleeping\n", i);
	usleep(time);
	//printf("Philo %i is thinking\n", i);
}

void	*philo_thread(void *void_philo)
{
	t_philo	*philo;

	philo = (t_philo *) void_philo;
	philo->meals_eaten = 3;
	while (philo->meals_eaten)
	{
		eat(philo, philo->forks);
		zzz(philo->rules.sleep_time, philo->i);
		philo->meals_eaten--;
	}
	pthread_exit(NULL);
	return (0);
}

void	get_rules(int ac, char *av[], t_rules *rules)
{
	if (ac != 5 && ac != 6)
	{
		printf("Wrong arguments!\n");
		return ;
	}
	rules->philo_nbr = ft_atoi(av[1]);
	rules->death_time = ft_atoi(av[2]);
	rules->eat_time = ft_atoi(av[3]);
	rules->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		rules->meal_nbr = ft_atoi(av[5]);
}

int	main(int ac, char *av[])
{
	int	i;
	pthread_t	*threads;
	pthread_mutex_t	*forks;
	t_philo	*philo;
	t_rules rules;

	get_rules(ac, av, &rules);
	threads = malloc(rules.philo_nbr * sizeof(pthread_t));
	forks = malloc(rules.philo_nbr * sizeof(pthread_mutex_t));
	philo = malloc(rules.philo_nbr * sizeof(t_philo));
	i = -1;
	while (++i < rules.philo_nbr)
		pthread_mutex_init(&forks[i], NULL);
	i = -1;
	while (++i < rules.philo_nbr)
	{
		philo[i].i = i;
		philo[i].rules = rules;
		philo[i].forks = forks;
        pthread_create(&threads[i], NULL, philo_thread, (void*) &philo[i]);
	}
	i = -1;
	while (++i < rules.philo_nbr)
    	pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&forks[i]);
	free(threads);
	free(forks);
	free(philo);
	return (0);
}
