##
## EPITECH PROJECT, 2017
## Makefile
## File description:
## a_makefile_for_projetcs
##

CC		=	gcc -g3

RM		=	rm -rf

CFLAGS	+=	-Wall -Wextra

CFLAGS	+=	-I./include

NAME	=	client

SRCS	=	src/main.c	\
			src/init.c	\

OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re