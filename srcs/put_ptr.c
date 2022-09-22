/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_hex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:06:26 by jleroux           #+#    #+#             */
/*   Updated: 2022/05/25 11:58:18 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	get_width(char *str, t_flags *flags)
{
	int	c;

	c = ft_strlen(str);
	if (flags->precision == 0 && str[0] == '0')
		c = 0;
	if (flags->precision > c)
		c = flags->precision;
	if (flags->hash)
		c += 2;
	if (flags->precision == -1 && flags->precision == '0' && c < flags->width)
		return (flags->width);
	if (str[0] == '-' && (c == flags->precision || c == flags->width))
		c++;
	return (c);
}

static void	print_precision(char *str, t_flags *flags, int min_maj)
{
	int	count;

	if (flags->precision == 0 && str[0] == '0')
		return ;
	count = ft_strlen(str);
	if (flags->hash && count && min_maj)
		write(1, "0X", 2);
	else if (flags->hash && count)
		write(1, "0x", 2);
	while (count < flags->precision)
		count += write(1, "0", 1);
	if (flags->hash)
		count += 2;
	while (flags->pad == '0' && count < flags->width)
		count += write(1, "0", 1);
	write(1, str, ft_strlen(str));
}

int	put_ptr(void *ptr, t_flags *flags)
{	
	char	*base;
	char	*str;
	int		count;

	count = 0;
	base = "0123456789abcdef";
	flags->hash = 1;
	str = ft_utoa_base((long long int) ptr, base);
	count = get_width(str, flags);
	if (flags->right_padded)
		print_precision(str, flags, 0);
	while (count < flags->width)
		count += write(1, " ", 1);
	if (!flags->right_padded)
		print_precision(str, flags, 0);
	free(str);
	return (count);
}
