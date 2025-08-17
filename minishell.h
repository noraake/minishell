/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:05 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/16 23:52:13 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define REDIR_IN 0
# define REDIR_OUT 1
# define APPEND 2
# define HEREDOC 3
// # define REDIR_FD 4

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef GC_H
#  define GC_H

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}						t_gc;

void					gc_init(t_gc *gc);
void					*gc_malloc(t_gc *gc, size_t size);
void					gc_add(t_gc *gc, void *ptr);
void					gc_free_all(t_gc *gc);
char					*gc_strdup(t_gc *gc, const char *src);
t_gc					*gc_create(void);
void					gc_destroy(t_gc *gc);

# endif

typedef struct s_token_ctx
{
	t_list				**tokens;
	const char			*line;
	t_gc				*gc;
	int					start;
	int					end;
}						t_token_ctx;

typedef struct s_split_ctx
{
	const char			*line;
	t_gc				*gc;
	t_list				**tokens;
	int					i;
	int					start;
	char				quote;
	int					len;
}						t_split_ctx;

typedef struct s_redir
{
	char				*file;
	int					type;
	bool				quoted;
	bool				expand;
	int					heredoc_fd;
	int					target_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_cmd
{
	char				**args;
	char				*cmd_path;
	char				*infile;
	char				*outfile;
	int					append;
	struct s_cmd		*next;
	t_redir				*redirections;
}						t_cmd;

typedef struct s_env
{
	char				**env;
	char				**export_only;
	t_gc				*gc;
	int					last_status;
}						t_env;

typedef struct s_pipe_context
{
	t_cmd				*cmd;
	t_env				*env;
	t_gc				*gc;
	pid_t				*pids;
	t_cmd				*current;
	int					prev_fd;
	int					cmd_index;
}						t_pipe_context;

typedef struct s_pipe_split_ctx
{
	char				*line;
	int					*start;
	int					i;
	char				*quote;
	t_list				**pieces;
	t_gc				*gc;
}						t_pipe_split_ctx;

// typedef struct s_wait_data
// {
// 	int					last_status;
// 	int					printed_sigint;
// }						t_wait_data;

typedef struct s_redir_ctx
{
	char				**args;
	bool				*was_quoted;
	t_env				*env;
	t_gc				*gc;
	int					*error;
}						t_redir_ctx;

typedef struct s_expander_ctx
{
	char				*str;
	int					i;
	char				*result;
	int					j;
	t_env				*env;
}						t_expander_ctx;

typedef struct s_heredoc_ctx
{
	char				*delimiter;
	bool				expand;
	t_env				*env;
	t_gc				*gc;
}						t_heredoc_ctx;

typedef struct s_context
{
	t_env				*env;
	t_gc				*gc;
}						t_context;

void					update_status(int status, t_env *env);
char					*read_input(t_gc *gc);
void					cleanup_and_exit(t_gc *gc, int status);
t_cmd					*process_redirections(t_cmd *cmd, bool *was_quoted,
							t_env *env, t_gc *gc);
t_cmd					*handle_empty_args(t_cmd *cmd, t_gc *gc);
int						is_pipe_char(char c);
int						is_space(char c);
int						expand_and_clean_args(char **args, t_context *ctx,
							bool *was_quoted);
int						is_redirect(char c);
int						is_builtin(char *cmd);
char					**list_to_array(t_list *lst, t_gc *gc);
int						check_syntax_errors(char *line);
int						check_invalid_redirections(char *line);
int						check_invalid_pipes(char *line);
int						check_quotes_closed(char *line);
bool					is_quoted(char *str);
bool					is_redir(char c);
t_list					*ft_lstnew_gc(t_gc *gc, void *content);
char					*gc_substr(t_gc *gc, const char *s, unsigned int start,
							size_t len);
char					**split_by_pipe_gc(char *line, t_gc *gc);
int						validate_quotes(const char *str);
int						is_redirection_string(char *arg);
int						handle_export_arg(char *arg, t_env *env, t_gc *gc);
char					*expand_variables_keep_quotes(char *str, t_env *env,
							t_gc *gc);
char					*remove_quotes(t_gc *gc, const char *str);
bool					has_invalid_args_after_redirection(char **args);
t_cmd					*parse_command(const char *command_str, t_env *env,
							t_gc *gc);
char					*expand_variables(char *str, t_env *env, t_gc *gc);
char					**split_arguments(char *s);
t_redir					*parse_redirections(t_redir_ctx *ctx);
char					*ft_strndup(const char *s, size_t n);
char					*get_env_value(t_env *env, char *var);
char					**clean_args_after_redirections(char **args, t_gc *gc);
void					free_redirections(t_redir *redir);
void					free_command_table(t_cmd **cmds);
int						ft_isalnum_custom(char c);
t_cmd					*parse_all_commands_gc(char **command_strings,
							t_env *env, t_gc *gc);
char					**split_path(t_gc *gc, char *path_str);
void					execute_pipeline(t_cmd *cmds, t_env *env, t_gc *gc);
char					*ft_strndup_gc(const char *s, size_t n, t_gc *gc);
int						is_builtin(char *cmd);
int						exec_builtin(t_cmd *cmd, t_env *env, t_gc *gc);
int						handle_redirections(t_cmd *cmd, t_gc *gc);
int						handle_heredoc(char *raw_delimiter, bool expand,
							t_env *env, t_gc *gc);
char					**split_operators(const char *line, t_gc *gc);
char					**env_to_array(t_env *env, t_gc *gc);
int						create_pipe(int *fds);
void					setup_exec_signals(void);
void					restore_signals(void);
char					*expand_with_quotes(char *str, t_env *env, t_gc *gc);
t_cmd					*init_cmd(char *cmd, char **args, t_gc *gc);
void					handle_sigint(int sig);
t_env					*get_env_for_signals(t_env *new_env);
t_gc					*get_gc(t_gc *new_gc);
void					handle(int sig);
void					cleanup_readline(void);
void					update_cd_env(t_env *env, char *oldpwd, char *newpwd,
							t_gc *gc);
void					loop_minishell(t_env *env);
void					setup_signals(void);
int						append_env_var(t_env *env, char *var, t_gc *gc);
char					*build_full_path(char *dir, char *cmd, t_gc *gc);
char					*ft_strjoin_gc(char *s1, char *s2, t_gc *gc);
char					*find_command_path(char *cmd, t_env *env, t_gc *gc);
void					print_cmd_not_found(char *cmd);
int						handle_append_redir(char *file);
int						handle_input_redir(char *file);
int						handle_output_redir(char *file);
int						handle_redirections_new(t_cmd *cmd, t_env *env,
							t_gc *gc);
// void					exec_child(t_exec_params *params);
void					exec_with_pipes(t_cmd *cmds, t_env *env, t_gc *gc);
int						process_input(char *line, t_env *env);
int						execute_builtin_with_redirection(t_cmd *cmd, t_env *env,
							t_gc *gc);
void					execute_single_cmd(t_cmd *cmds, t_env *env, t_gc *gc);
int						builtin_cd(char **args, t_env *env, t_gc *gc);
int						builtin_echo(char **args);
int						builtin_pwd(void);
int						builtin_exit(char **args, t_gc *gc);
int						builtin_export(char **args, t_env *env, t_gc *gc);
int						builtin_unset(char **args, t_env *env);
int						builtin_env(t_env *env);
int						ft_strcmp(const char *s1, const char *s2);
t_gc					*gc_create(void);
bool					is_assignment(const char *s);
int						add_or_update_env_var(t_env *env, char *var, t_gc *gc);
int						handle_assignment(const char *arg, t_env *env,
							t_gc *gc);
void					collapse_empty_args(char **args);
int						print_error_invalid(const char *arg);
bool					is_valid_name(const char *name);
char					*normalize_escaped_name(const char *name, t_gc *gc);
char					*make_assignment(const char *name, const char *value);
int						print_exported_vars(t_env *env);
int						handle_export_with_value(char *name, char *value,
							t_env *env, t_gc *gc);
int						handle_export_without_value(char *name, t_env *env,
							t_gc *gc);
int						handle_append_export(char *name, char *value,
							t_env *env, t_gc *gc);
int						validate_and_normalize_name(char **name, char *value,
							char *arg, t_gc *gc);
int						extract_export_name_and_value(char *arg, char **name,
							char **value);
int						is_llong(const char *str);
int						print_invalid_identifier_error(const char *arg);
int						is_numeric(char *str);
int						remove_env_var(t_env *env, char *key);
bool					has_quotes(const char *s);
int						has_exec_permission(const char *path);
void					child_exec_single(t_cmd *cmds, t_env *env, t_gc *gc);
void					set_signals_for_redirection(void);
void					handle_signaled_status(int status, t_env *env);
void					handle_redirection_exit_status(int status, t_env *env);
void					handle_redirection_only(t_cmd *cmds, t_env *env,
							t_gc *gc);
void					wait_and_set_status(int status, t_env *env);
void					handle_exec(t_cmd *cmds, t_env *env, t_gc *gc);
t_cmd					*init_cmd_and_parse_args(const char *command_str,
							t_env *env, t_gc *gc, bool **was_quoted_out);
t_redir					*create_redirection(char *op, char *file, bool quoted,
							t_gc *gc);
void					print_ambiguous_redirect(char *filename);
int						print_redirection_error(char *msg);
bool					has_command(char **args);
int						is_redirection(char *arg);
int						count_cmds(t_cmd *cmds);
int						handle_heredoc_result(int pipefd[2], pid_t pid,
							t_env *env);
int						handle_redir_entry(t_redir *redir, int *last_fd,
							t_env *env, t_gc *gc);
int						handle_heredoc_redir(t_redir *redir, int *last_fd,
							t_env *env, t_gc *gc);
int						handle_out_append_redir(t_redir *redir);
int						handle_in_redir(t_redir *redir, int *last_fd);
int						handle_redirection_validation(char *line, int *i);
int						handle_pipe_validation(char *line, int *i,
							bool *first_non_space);
bool					is_redir(char c);
void					init_token_ctx(t_token_ctx *ctx, t_list **tokens,
							const char *line, t_gc *gc);
void					add_token(t_token_ctx *ctx);
int						handle_redirection(const char *line, int i,
							t_list **tokens, t_gc *gc);
void					handle_quote(char c, char *quote);
int						is_quote(char c);
void					add_pipe_if_needed(t_pipe_split_ctx *ctx);
int						check_numeric_range(const char *str, int sign);
bool					update_quote_state(char c, bool *in_single,
							bool *in_double);
void					handle_variable(t_expander_ctx *ctx);
bool					update_quote_state(char c, bool *in_single,
							bool *in_double);
int						calculate_expanded_size(char *str, t_env *env);
void					process_char_in_expand(t_expander_ctx *ctx);
int						process_var_size(char *str, int *i, t_env *env);
t_redir					*handle_fd_redirection(char **args, int *i, t_gc *gc,
							int *error);
int						add_to_export_only(t_env *env, const char *name,
							t_gc *gc);
void					init_pipe_context(t_pipe_context *ctx, t_cmd *cmds,
							t_env *env, t_gc *gc);
void					close_fd_if_valid(int fd);
void					manage_fds_after_fork(t_pipe_context *ctx,
							int pipefd[2]);
void					handle_command_execution(t_pipe_context *ctx);
void					setup_and_fork(t_pipe_context *ctx, int pipefd[2]);
void					exec_child_process(t_pipe_context *ctx, int pipefd[2]);
void					exec_external_cmd(t_pipe_context *ctx);
void					process_char_in_heredoc_expand(t_expander_ctx *ctx);
char					*expand_variables_heredoc(char *str, t_env *env,
							t_gc *gc);
void					exec_with_checks_exit(char *path, t_cmd *cmd,
							t_env *env);
char					*ft_strcpy(char *dest, const char *src);

#endif