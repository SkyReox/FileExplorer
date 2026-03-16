##
## EPITECH PROJECT, 2026
## FileExplorer
## File description:
## Makefile
##

SRC =	src/FileExplorer.cpp			\
		src/FileBar.cpp					\
		src/RoundedRectangleShape.cpp	\
		src/Button.cpp					\
		src/DirButton.cpp				\

MAIN =	src/main.cpp

OBJ = $(SRC:.cpp=.o)

MAIN_OBJ = $(MAIN:.cpp=.o)

NAME = FileExplorer

CPPFLAGS = -I./include

LFLAGS = -lsfml-graphics -lsfml-system -lsfml-window

ERRORSFLAGS = -W -Wall -Werror -Wextra

COMPILER = g++ -std=c++20

all: $(NAME)

$(NAME): $(MAIN_OBJ) $(OBJ)
	$(COMPILER) $(ERRORSFLAGS) -o $(NAME) $(CPPFLAGS) $(MAIN_OBJ) $(OBJ) $(LFLAGS)

clean:
	rm -f $(MAIN_OBJ)
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
