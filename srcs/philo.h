/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:13:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/13 11:51:36 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

//Change to enum ?
# define EATING 0
# define SLEEPING 1

typedef struct s_rules
{
	int			philo_nbr;
	int			death_time;
	int			eat_time;
	int			sleep_time;
	int			max_meal;
	long long	start_time;
}				t_rules;

typedef struct s_philo
{
	int				id;
	int				state;
	int				meals_eaten;
	t_rules			rules;
	pthread_mutex_t	*frks;
}				t_ph;

int	ft_atoi(const char *str);

#endif
