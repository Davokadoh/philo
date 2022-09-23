/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 15:20:14 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/23 15:23:00 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	now(int init)
{
	struct timeval		time;
	static long			start;
	long				milliseconds;

	gettimeofday(&time, NULL);
	milliseconds = time.tv_sec * 1000L + time.tv_usec / 1000;
	if (init)
		start = milliseconds;
	return (milliseconds - start);
}

void	msleep(long long msec)
{
	struct timeval	time;
	long			start;
	long			now;

	gettimeofday(&time, NULL);
	now = time.tv_sec * 1000L + time.tv_usec / 1000;
	start = now;
	while (now - start < msec)
	{
		usleep(100);
		gettimeofday(&time, NULL);
		now = time.tv_sec * 1000L + time.tv_usec / 1000;
	}
}
