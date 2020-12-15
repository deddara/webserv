# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/15 10:44:20 by awerebea          #+#    #+#              #
#    Updated: 2020/12/15 12:42:48 by awerebea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
INCLUDES	= -I includes/ -I src/
CC			= clang++
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -O2
DBGFLAGS	= -g

override FLAGS ?= $(CFLAGS)

#------------------------------------ compiling --------------------------------
SRCDIR		= src/
OBJDIR		= obj/

FLSDIR_1	=
FLS_1		= $(addprefix $(FLSDIR_1), \
				ConfParser \
				)
FLSDIR_2	= tests/
FLS_2		= $(addprefix $(FLSDIR_2), \
				test_ConfParser \
				)

SRC	= $(FLS_1) $(FLS_2)
# SRC	= $(FLS_1)

OBJ	= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS	= $(SRC:=.d)

all:		$(NAME)

$(NAME):	$(OBJ)
	$(CC)		$(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
	@echo '------------- All done! --------------'

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(FLSDIR_1) $(FLSDIR_2))
	# mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(FLSDIR_1))
	$(CC)		$(FLAGS) $(INCLUDES) -c $< -o $@ -MMD

include $(wildcard $(addprefix $(OBJDIR), $(DFLS)))

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
	debug \
	fclean \
	re \
	run \
	test \
	test_valgrind
