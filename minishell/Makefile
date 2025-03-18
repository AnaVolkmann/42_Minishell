# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/16 19:13:13 by ana-lda-          #+#    #+#              #
#    Updated: 2025/03/18 11:18:23 by ana-lda-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
READLINE_INCLUDE = $(shell brew --prefix readline)/include
READLINE_LIB = $(shell brew --prefix readline)/lib
INCLUDES = -I./includes -I./lib/libft -I./lib/get_next_line -I$(READLINE_INCLUDE)
SRCDIR = ./src
OBJDIR = ./obj
LIBFTDIR = ./lib/libft
LIBGNTLDIR = ./lib/get_next_line
READLINEDIR = ./lib/readline

SRCS = $(SRCDIR)/main.c \
	$(SRCDIR)/input_validation/syntax_checker.c \
	$(SRCDIR)/input_validation/syntax_checker_utils.c \
	$(SRCDIR)/tokenization/tokenizer.c \
	$(SRCDIR)/tokenization/tokenizer_utils.c \
	$(SRCDIR)/parsing/parser.c \
	$(SRCDIR)/parsing/parser_utils.c \
	$(SRCDIR)/execution/initialization/command_execution_init.c \
	$(SRCDIR)/execution/initialization/environment_init.c \
	$(SRCDIR)/execution/builtin_command_execution.c \
	$(SRCDIR)/execution/builtin_command_execution2.c \
	$(SRCDIR)/execution/command_execution_adv.c \
	$(SRCDIR)/execution/executor.c \
	$(SRCDIR)/execution/path_utilities.c \
	$(SRCDIR)/execution/path_utilities2.c \
	$(SRCDIR)/execution/process_pipeline_management.c \
	$(SRCDIR)/execution/command_execution_control.c \
	$(SRCDIR)/execution/command_input_preprocessing.c \
	$(SRCDIR)/execution/heredoc_handling.c \
	$(SRCDIR)/builtin/builtin_commands.c \
	$(SRCDIR)/builtin/builtin_commands_utils.c \
	$(SRCDIR)/builtin/builtin_commands_utils2.c \
	$(SRCDIR)/builtin/builtin_commands_utils3.c \
	$(SRCDIR)/signals/signal_handlers.c \
	$(SRCDIR)/env_var_expansion/env_handling.c \
	$(SRCDIR)/env_var_expansion/env_managment.c \
	$(SRCDIR)/env_var_expansion/env_managment2.c \
	$(SRCDIR)/env_var_expansion/variable_expansion.c \
	$(SRCDIR)/env_var_expansion/variable_expansion_utils.c \
	$(SRCDIR)/utils/string_utilities/string_utils.c \
	$(SRCDIR)/utils/string_utilities/string_utils2.c \
	$(SRCDIR)/utils/shell_utilities.c \

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LDFLAGS = -L$(LIBFTDIR) -lft -L$(READLINE_LIB) -lreadline
LIBFT = $(LIBFTDIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@printf "\r \e[1;32mMINISHELL\e[0m compiled successfully\n"

$(OBJDIR)/%.o: $(SRCDIR)/%.c ./includes/minishell.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

clean:
	rm -rf $(OBJDIR)
	$(MAKE) -C $(LIBFTDIR) clean
	@printf " \e[1;33mObjects Removed\n\e[0m"

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean
	@printf " \e[1;31mExecutable Deleted\n\e[0m"

re: fclean all
	@printf "\r \e[1;32mRefresh OK!\e[0m\n"

.PHONY: all clean fclean re
