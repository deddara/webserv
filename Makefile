# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/15 10:44:20 by awerebea          #+#    #+#              #
#    Updated: 2020/12/18 15:01:49 by awerebea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
INCLUDES	= -I includes/
CXX			= clang++

CFLAGS		= -Wall -Wextra -Werror
# CFLAGS		= -Wall -Wextra
OFLAGS		= -O2
DBGFLAGS	= -g

override FLAGS ?= $(CFLAGS)

#------------------------------------ compiling --------------------------------
SRCDIR		=
OBJDIR		= obj/

DIR_ROOT	=
FLS_ROOT	= $(addprefix $(DIR_ROOT), \
				main \
				)

DIR_1		= conf_parse/
FLS_1		= $(addprefix $(DIR_1), \
				ConfParser \
				Location \
				VirtServer \
				)
INCLUDES	+= -I $(DIR_1)

DIR_2		= request_parse/
FLS_2		= $(addprefix $(DIR_2), \
				Request \
				)
INCLUDES	+= -I $(DIR_2)

DIR_3		= server_engine/
FLS_3		= $(addprefix $(DIR_3), \
				Server \
				)
INCLUDES	+= -I $(DIR_3)

DIR_4		= utils/
FLS_4		= $(addprefix $(DIR_4), \
				time \
				utils \
				)
INCLUDES	+= -I $(DIR_4)

DIR_TEST	= tests/
FLS_TEST	= $(addprefix $(DIR_TEST), \
				test_ConfParser \
				)

SRC			= $(FLS_ROOT) $(FLS_1) $(FLS_2) $(FLS_3) $(FLS_4)

OBJ			= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS		= $(SRC:=.d) $(SRC_C:=.d)

all:		$(NAME)

$(NAME):	$(OBJ)
	$(CXX)		$(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
	@echo '------------- All done! --------------'

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(FLS_ROOT) $(FLS_1) $(FLS_2) $(FLS_3) $(FLS_4))
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
