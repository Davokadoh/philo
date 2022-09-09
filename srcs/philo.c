/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:11:47 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/09 15:38:04 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_philo
{
	int	i;
}				t_philo;

void	eat()
{
	take_left_fork();
	take_right_fork();
	print_eat();
	put_forks_back();
}

void	sleep()
{
	printf("Sleep\n");
	usleep();
	printf("Think\n");
}

void	*myThreadFunc(void *void_philo)
{
	t_philo	*philo;
	int	dead;

	//pthread_detach(pthread_self());
	dead = 1;
	while (!dead)
	{
		eat();
		sleep();
	}
	philo = (t_philo *) void_philo;
	sleep(1);
	printf("%d\n", philo->i);
	pthread_exit(NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	pthread_t	thread_id;
	int	i;
	int	philo_nbr;
	int	death_time;
	int	eat_time;
	int	sleep_time;
	int	meal_nbr;
	int	*forks;
	t_philo	*philo;

	if (ac != 5 && ac != 6)
		return (printf("Wrong arguments!\n"));
	philo_nbr = ft_atoi(av[1]);
	death_time = ft_atoi(av[2]);
	eat_time = ft_atoi(av[3]);
	sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		meal_nbr = ft_atoi(av[5]);
	forks = malloc(philo_nbr * sizeof(int));
	philo = malloc(philo_nbr * sizeof(t_philo));
	i = -1;
	while (++i < philo_nbr)
	{
		philo[i].i = i;
        pthread_create(&thread_id, NULL, myThreadFunc, (void*) &(philo[i]));
	}
    pthread_join(thread_id, NULL);
	free(forks);
	free(philo);
	return (0);
}
