NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -I.
RM = rm -rf

SRCS = main.c \
		Codexion.h \
		src/utils.c \
		src/monitor.c \
		src/parse.c \
		src/init.c \
		src/routine.c \
		
		

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c Codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re