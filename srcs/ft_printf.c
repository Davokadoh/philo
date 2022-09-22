/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 11:59:29 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/22 13:39:37 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	parse_type(char const *input_str, va_list args, t_flags *flags)
{
	if (!*input_str)
		return (0);
	else if (*input_str == 'c')
		return (put_char(va_arg(args, int), flags));
	else if (*input_str == 's')
		return (put_str(va_arg(args, char *), flags));
	else if (*input_str == 'p')
		return (put_ptr(va_arg(args, void *), flags));
	else if (*input_str == 'd' || *input_str == 'i')
		return (put_int(va_arg(args, int), flags));
	else if (*input_str == 'u')
		return (put_u_int(va_arg(args, unsigned int), flags));
	else if (*input_str == 'x')
		return (put_hex(va_arg(args, unsigned int), flags, 0));
	else if (*input_str == 'X')
		return (put_hex(va_arg(args, unsigned int), flags, 1));
	else if (*input_str == '%')
		return (put_char('%', flags));
	else
		return (0);
}

static t_flags	init_flags(void)
{
	t_flags	flags;

	flags.width = 0;
	flags.right_padded = 0;
	flags.pad = ' ';
	flags.hash = 0;
	flags.space = 0;
	flags.plus = 0;
	flags.precision = -1;
	return (flags);
}

static void	parse_precision(char **input_str, t_flags *flags)
{
	(*input_str)++;
	flags->precision = ft_atoi(input_str);
	(*input_str)--;
}

static t_flags	parse_flags(char **input_str)
{
	t_flags	flags;

	flags = init_flags();
	while (ft_strchr("-0# +.123456789", **input_str))
	{
		if (ft_strchr("123456789", **input_str))
		{
			flags.width = ft_atoi(input_str);
			(*input_str)--;
		}
		else if (**input_str == '-')
			flags.right_padded = 1;
		else if (**input_str == '0')
			flags.pad = '0';
		else if (**input_str == '#')
			flags.hash = 1;
		else if (**input_str == ' ')
			flags.space = 1;
		else if (**input_str == '+')
			flags.plus = 1;
		else if (**input_str == '.')
			parse_precision(input_str, &flags);
		(*input_str)++;
	}
	return (flags);
}

int	m_printf(pthread_mutex_t *m_write, const char *input_str, ...)
{
	va_list	args;
	t_flags	flags;
	int		len;

	len = 0;
	va_start(args, input_str);
	pthread_mutex_lock(m_write);
	while (*input_str)
	{
		if (*input_str == '%' && input_str[1])
		{
			input_str++;
			if (*input_str)
			{
				flags = parse_flags((char **) &input_str);
				len += parse_type(input_str, args, &flags);
			}
		}
		else
			len += write(1, input_str, 1);
		input_str++;
	}
	pthread_mutex_unlock(m_write);
	va_end(args);
	return (len);
}
