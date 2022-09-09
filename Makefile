CFLAGS 			=	-Wall -Wextra -Werror #-fsanitize=address
TARGET 			=	philo
SRCS_DIR 		=	./srcs
OBJS_DIR 		=	./objs
LIBS			=	-lpthread
_SRCS 			=	philo.c \
					ft_atoi.c
_SRCS_BONUS		=	philo_bonus.c

OBJS_BONUS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(_SRCS_BONUS))
	
OBJS = $(patsubst %.c, $(OBJS_DIR)/%.o, $(_SRCS))
SRCS = $(patsubst %, $(SRCS_DIR)/%, $(_SRCS))

.PHONY : all clean fclean re bonus

all: $(TARGET)

bonus: $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS_BONUS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS_BONUS)
	rmdir $(OBJS_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

$(OBJS_DIR):
	@mkdir -p $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	@$(CC) -c $(CFLAGS) -o $@ $^

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
