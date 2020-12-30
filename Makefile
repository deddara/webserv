# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/15 10:44:20 by awerebea          #+#    #+#              #
#    Updated: 2020/12/30 16:48:23 by awerebea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
CONFIG		= webserv.conf

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
				# main_request
INCLUDES	+= -I $(DIR_2)

DIR_3		= server_engine/
FLS_3		= $(addprefix $(DIR_3), \
				Bytes \
				Chunk \
				Client \
				ErrorPages \
				Response \
				Server \
				)
INCLUDES	+= -I $(DIR_3)

DIR_4		= utils/
FLS_4		= $(addprefix $(DIR_4), \
				ft_atoi \
				ft_itoa \
				ft_memcmp \
				ft_memcpy \
				ft_strdup \
				ft_strjoin \
				ft_strnstr \
				time \
				utils \
				)
INCLUDES	+= -I $(DIR_4)

DIR_TEST	= tests/
# FLS_TEST	= $(addprefix $(DIR_TEST), \
#                 test_ConfParser \
#                 )

SRC			= $(FLS_ROOT) $(FLS_1) $(FLS_2) $(FLS_3) $(FLS_4)
DIRS		= $(DIR_1) $(DIR_2) $(DIR_3) $(DIR_4)

OBJ			= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS		= $(SRC:=.d) $(SRC_C:=.d)

REQUIRED_BINS	= python

all:		requerments $(NAME)

	# | sed 's!PHP_CGI!$(shell which php-cgi)!g' \

requerments:
	$(foreach bin,$(REQUIRED_BINS),\
		$(if $(shell command -v $(bin) 2> /dev/null),$(info Found '$(bin)'),$(error Please install '$(bin)')))
	@cat webserv.conf.template | sed 's=PWD=$(PWD)=g' \
	| sed 's!PYTHON!$(shell which python)!g' > webserv.conf
	@printf '\033[1;36mDefault config file $(CONFIG) created.\033[0m\n'

$(NAME):	dirs $(OBJ)
	@$(CXX)		$(FLAGS) $(OBJ) $(INCLUDES) -o $(NAME)
	@printf '\033[1m------------- All done! --------------\033[0m\n'

dirs:
	@mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(DIRS))

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@$(CXX)		$(FLAGS) $(INCLUDES) -c $< -o $@ -MMD

include $(wildcard $(addprefix $(OBJDIR), $(DFLS)))

clean_all: clean libft_clean

clean:
	rm -rf	$(OBJDIR)

fclean:	clean
	rm -f	$(NAME)
	rm -f	$(CONFIG)

debug:
	make FLAGS="$(CFLAGS) $(DBGFLAGS)" all

run: all
	./$(NAME)

test_ConfParser:
	@make	FLAGS="-Wall -Wextra -w $(DBGFLAGS)" \
			SRC="$(addprefix $(DIR_TEST), test_ConfParser) $(FLS_1) $(FLS_4)" \
			DIRS="$(DIR_TEST) $(DIR_1) $(DIR_4)" \
			all --no-print-directory
	./$(NAME)

test_resp_prepare:
	@make	FLAGS="-Wall -Wextra -w $(DBGFLAGS)" \
			SRC="$(addprefix $(DIR_TEST), test_resp_prepare) $(FLS_1) $(FLS_2) \
			$(FLS_3) $(FLS_4)" \
			DIRS="$(DIR_TEST) $(DIR_1) $(DIR_2) $(DIR_3) $(DIR_4)" \
			all --no-print-directory

test_valgrind: test_resp_prepare
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

re:	fclean all

.PHONY:	all \
		clean \
		clean_all \
		debug \
		fclean \
		re \
		run \
		test_ConfParser \
		test_valgrind
