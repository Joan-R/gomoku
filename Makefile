##
## EPITECH PROJECT, 2024
## B-AIA-500-MAR-5-1-gomoku-nattan.cochet
## File description:
## Makefile
##

NAME	=	pbrain-gomoku-ai

SRC	=	main.cpp	\
		board.cpp   \
		minimax.cpp	\

FLAGS	=	-Wall -Wextra -std=c++20 -g3 -g

FLAGS_PROFILE	=	-pg

all	:
	g++ $(FLAGS) $(SRC) -o $(NAME)

profiling :
	g++ $(FLAGS) $(FLAGS_PROFILE) $(SRC) -o $(NAME)

clean	:
	rm -f $(NAME)
	rm -f *.o

fclean	:	clean

re	:	fclean all