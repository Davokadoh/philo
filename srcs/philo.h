/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:13:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/19 14:14:39 by jleroux          ###   ########.fr       */
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
	int				finished;
	pthread_mutex_t	death_mutex;
}				t_data;

typedef struct s_philo
{
	int				id;
	int				state;
	int				meals_eaten;
	int				has_forks;
	long long		last_meal;
	t_data			*data;
	pthread_mutex_t	*frks;
}				t_ph;

int	ft_atoi(const char *str);

#endif
