# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jleroux <marvin@42lausanne.ch>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/07 13:26:50 by jleroux           #+#    #+#              #
#    Updated: 2022/05/25 11:34:00 by jleroux          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libftprintf.a

CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror

AR		=	ar

ARFLAGS	=	rcs

SRCS	=	ft_printf.c\
		   	put_char.c\
			put_str.c\
			put_int.c\
			put_u_int.c\
			put_hex.c\
			put_ptr.c\
			ft_atoi.c\
			ft_itoa.c\
			ft_itoa_base.c\
			ft_strchr.c\
			ft_strlen.c

OBJS	=	$(SRCS:.c=.o)

%.o	:	%.c
			$(CC) $(CFLAGS) -c $< -o $@

RM		=	rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
			$(AR) $(ARFLAGS) $(NAME) $(OBJS)

bonus:		$(OBJS)
			$(AR) $(ARFLAGS) $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

test:		all
			$(CC) main.c libftprintf.a

.PHONY:		all clean fclean re
