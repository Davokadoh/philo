/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 13:13:22 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/27 11:21:25 by jleroux          ###   ########.fr       */
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
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_end;
}				t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				finished;
	int				has_forks;
	long			last_meal;
	t_data			*data;
	pthread_mutex_t	*frks;
	pthread_mutex_t	m_last_meal;
}				t_ph;

int		ft_atoi(char **str);
int		ft_error(char *msg, int errno);
int		is_end(t_ph *ph);
int		is_finished(t_ph *ph);
int		malloc_arr(int nb, t_ph **ph, pthread_t **tids, pthread_mutex_t **frks);
void	free_arrays(t_ph *ph, pthread_t *tids, pthread_mutex_t *frks);
void	destroy_mutexes(t_data *data, pthread_t *tids, pthread_mutex_t *frks);
void	msleep(long long msec);
void	die(t_ph *ph);
void	eat(t_ph *ph);
void	zzz(t_ph *ph);
void	think(t_ph *ph);
void	take_forks(t_ph *ph, pthread_mutex_t *frks);
void	drop_forks(t_ph *ph, pthread_mutex_t *frks);
void	death_check(t_ph *ph, int max);
void	m_print(t_ph *ph, char *msg);
long	now(int init);

#endif
