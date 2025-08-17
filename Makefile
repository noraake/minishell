NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RLFLAG = -lreadline

# Libft
LIBFT_DIR = Libft
LIBFT_A = $(LIBFT_DIR)/libft.a
LIBFT_FLAG = -L$(LIBFT_DIR) -lft

SRCS = aaloc.c main.c read_inp.c split_pipes.c syntax_check.c utils_syntax_check.c \
       command_parser.c pars_pars.c expander.c quote_handler.c split_args.c parser_utils.c \
       free_parser.c redirection_parser.c utils.c exec_pipe_utils.c env_utils.c executor.c builtin.c \
       redirections.c heredoc.c is_long.c env_exec_utils.c pipe_utils.c signals_exec.c execution_structs.c \
	   split_path.c loop.c loop2.c builtin2.c builtin3.c exec_utils.c exec_builtin.c spliiit.c gc.c assignment_utils.c builtin_export.c \
	   print_error.c builtin_export_utils.c builtin_exit.c utils2.c builtin_cd.c exec_child.c \
	   handle_redirections.c exec_single_cmd.c child_single.c exec_with_pipes.c command_parser_utils.c \
	   redirection_helpers.c redirection_create.c redirection_utils.c heredoc2.c process_redirection.c syntax_check2.c token_utils.c \
	   redirection_handling.c pipe_split_utils.c expander_utils.c expander_utils2.c expander_utils3.c builtin_export_utils2.c expander_heredoc.c \

OBJS = $(SRCS:.c=.o)

all: $(LIBFT_A) $(NAME)

$(NAME): $(LIBFT_A) $(LIBFT_B) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(RLFLAG) $(LIBFT_FLAG) -o $(NAME)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
