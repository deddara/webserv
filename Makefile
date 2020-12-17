# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/15 10:44:20 by awerebea          #+#    #+#              #
#    Updated: 2020/12/17 20:37:57 by awerebea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
INCLUDES	= -I includes/ -I src/
CXX			= clang++

CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -O2
DBGFLAGS	= -g

override FLAGS ?= $(CFLAGS)

#------------------------------------ compiling --------------------------------
SRCDIR		= src/
OBJDIR		= obj/

DIR_1		=
FLS_1		= $(addprefix $(DIR_1), \
				ConfParser \
				Location \
				VirtServer \
				)

DIR_TEST	= tests/
FLS_TEST	= $(addprefix $(DIR_TEST), \
				test_ConfParser \
				)

SRC			= $(FLS_1) $(FLS_TEST)

OBJ			= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS		= $(SRC:=.d) $(SRC_C:=.d)

all:		$(NAME)

$(NAME):	$(OBJ)
	$(CXX)		$(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
	@echo '------------- All done! --------------'

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(DIR_1) $(DIR_TEST))
	$(CXX)		$(FLAGS) $(INCLUDES) -c $< -o $@ -MMD

include $(wildcard $(addprefix $(OBJDIR), $(DFLS)))

clean_all: clean libft_clean

clean:
	rm -rf	$(OBJDIR)

fclean:	clean
	rm -f	$(NAME)

debug:
	make FLAGS="$(CFLAGS) $(DBGFLAGS)" all

run: all
	./$(NAME)

test: debug
	./$(NAME)

test_valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

re:	fclean all

.PHONY:	all \
		clean \
		clean_all \
		debug \
		fclean \
		re \
		run \
		test \
		test_valgrind
