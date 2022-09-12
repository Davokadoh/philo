/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/12 14:31:49 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	HUNGRY 0
#define	EATING 1
#define	SLEEPING 2
#define	THINKING 3
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
	t_rules			rules;
	pthread_mutex_t	*forks;
}				t_philo;

void	take_fork(t_philo philo, int frk, pthread_mutex_t *forks)
{
	if (frk >= philo.rules.philo_nbr)
		frk = 0;
	printf("Philo %i tries to take fork %i\n", philo.i, frk);
	pthread_mutex_lock(&forks[frk]);
	printf("Philo %i has taken fork %i\n", philo.i, frk);
}

void	eat(t_philo *philo, pthread_mutex_t *forks)
{
	if (philo->i == philo->rules.philo_nbr)
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
	int	death_counter;

	death_counter = 5;
	philo = (t_philo *) void_philo;
	if (philo->i == 0)
		philo->state = EATING;
	while (death_counter)
	{
		eat(philo, philo->forks);
		zzz(philo->rules.sleep_time, philo->i);
		death_counter--;
	}
	pthread_exit(NULL);
	return (0);
}


int	main(int ac, char *av[])
{
	int	i;
	pthread_t	*threads;
	pthread_mutex_t	*forks;
	t_philo	*philo;
	t_rules rules;

	if (ac != 5 && ac != 6)
		return (printf("Wrong arguments!\n"));
	rules.philo_nbr = ft_atoi(av[1]);
	rules.death_time = ft_atoi(av[2]);
	rules.eat_time = ft_atoi(av[3]);
	rules.sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		rules.meal_nbr = ft_atoi(av[5]);
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
		philo[i].state = HUNGRY;
		philo[i].rules = rules;
		philo[i].forks = forks;
        pthread_create(&threads[i], NULL, philo_thread, (void*) &philo[i]);
	}
	i = -1;
	while (++i < rules.philo_nbr)
    	pthread_join(threads[i], NULL);
	free(forks);
	free(philo);
	return (0);
}
