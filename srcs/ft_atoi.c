/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:37:46 by jleroux           #+#    #+#             */
/*   Updated: 2022/05/25 14:30:32 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_atoi(char **str)
{
	int						sign;
	long long unsigned int	nbr;

	sign = 1;
	nbr = 0;
	while (**str == 32 || (9 <= **str && **str <= 13))
		*str += 1;
	if (**str == 43 || **str == 45)
	{
		if (**str == 45)
			sign = -1;
		*str += 1;
	}
	while (48 <= **str && **str <= 57)
	{
		nbr *= 10;
		nbr += **str - '0';
		*str += 1;
	}
	if (nbr > 4294967295 && sign == -1)
		return (0);
	if (nbr > 4294967295 && sign == 1)
		return (-1);
	return (nbr * sign);
}
