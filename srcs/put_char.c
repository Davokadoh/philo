/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_char.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:06:17 by jleroux           #+#    #+#             */
/*   Updated: 2022/05/25 11:35:51 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	put_char(char ch, t_flags *flags)
{
	int	count;

	count = 0;
	if (flags->right_padded)
		count += write(1, &ch, 1);
	while (count < flags->width)
		count += write(1, " ", 1);
	if (!flags->right_padded)
		count += write(1, &ch, 1);
	return (count);
}
