/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 11:50:01 by jleroux           #+#    #+#             */
/*   Updated: 2022/05/25 11:35:39 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	len_in_base(long long unsigned int n, unsigned int b_len)
{
	if (n >= b_len)
		return (len_in_base(n / b_len, b_len) + 1);
	else
		return (1);
}

static void	add_digit(long long unsigned int n, char *str, char *base)
{
	unsigned int	b_len;

	b_len = ft_strlen(base);
	*str = base[n % b_len];
	if (n >= b_len)
		add_digit(n / b_len, --str, base);
	return ;
}

char	*ft_itoa_base(long long int n, char *base)
{
	char	*str;
	int		b_len;
	int		len;

	b_len = ft_strlen(base);
	if (n < 0)
		len = len_in_base(-n, b_len) + 1;
	else
		len = len_in_base(n, b_len);
	str = (char *) malloc(len * sizeof(char) + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	if (n < 0)
		add_digit(-n, &str[--len], base);
	else
		add_digit(n, &str[--len], base);
	if (n < 0)
		str[0] = '-';
	return (str);
}

char	*ft_utoa_base(long long unsigned int n, char *base)
{
	char	*str;
	int		b_len;
	int		len;

	b_len = ft_strlen(base);
	len = len_in_base(n, b_len);
	str = (char *) malloc(len * sizeof(char) + 1);
	if (!str)
		return (NULL);
	str[len] = 0;
	add_digit(n, &str[--len], base);
	return (str);
}
