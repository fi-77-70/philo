NAME = philo

SRC = philo.c parsing.c utils.c

COMPILE = cc -Wall -Werror -Wextra -g -fsanitize=address

all: $(NAME)

$(NAME): $(SRC)
		$(COMPILE) $(SRC) -o $(NAME)

re: fclean all

clean:

fclean:
		rm -f philo
