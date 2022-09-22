/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 15:36:32 by jleroux           #+#    #+#             */
/*   Updated: 2022/09/22 13:40:14 by jleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

typedef struct s_flags
{
	int		width;
	int		right_padded;
	int		precision;
	int		hash;
	int		space;
	int		plus;
	char	pad;
}				t_flags;

int		put_char(char ch, t_flags *flags);
int		put_str(char *str, t_flags *flags);
int		put_ptr(void *ptr, t_flags *flags);
int		put_int(long long int nbr, t_flags *flags);
int		put_u_int(unsigned int nbr, t_flags *flags);
int		put_hex(long long int nbr, t_flags *flags, int min_maj);
int		ft_isdigit(char const ch);
int		ft_atoi(char **str);
int		ft_strlen(char *str);
char	*ft_strdup(char *src);
char	*ft_itoa(long long int n);
char	*ft_itoa_base(long long int n, char *base);
char	*ft_utoa_base(long long unsigned int n, char *base);
char	*ft_strchr(const char *s, int c);
int		m_printf(pthread_mutex_t *m_write, const char *input_str, ...);

#endif
