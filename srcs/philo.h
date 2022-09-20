/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:13:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/20 14:59:00 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data
{
	int				nbr;
	int				dead;
	int				death_time;
	int				eat_time;
	int				zzz_time;
	int				max_meal;
	int				all_finished;
	pthread_mutex_t	finished_mutex;
	pthread_mutex_t	death_mutex;
}				t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				has_forks;
	long			last_meal;
	t_data			*data;
	pthread_t		death_timer;
	pthread_mutex_t	*frks;
	pthread_mutex_t	last_meal_mutex;
}				t_ph;

int	ft_atoi(const char *str);

#endif
