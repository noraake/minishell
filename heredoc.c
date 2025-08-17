/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:56:46 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 04:55:45 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_heredoc_line(t_heredoc_ctx *ctx, int write_fd, char *line)
{
	if (!line)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("minishell: warning: here-document delimited ");
			printf("by end-of-file (wanted `%s')\n", ctx->delimiter);
		}
		close(write_fd);
		if (ctx->gc)
			gc_destroy(ctx->gc);
		exit(0);
	}
	if (!ft_strcmp(line, ctx->delimiter))
	{
		if (ctx->gc)
			gc_destroy(ctx->gc);
		exit(0);
	}
	if (ctx->expand)
		line = expand_variables_heredoc(line, ctx->env, ctx->gc);
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

void	child_heredoc_process(t_heredoc_ctx *ctx, int write_fd)
{
	char	*line;

	get_gc(ctx->gc);
	signal(SIGINT, handle);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline(">> ");
		process_heredoc_line(ctx, write_fd, line);
		free(line);
	}
}

int	handle_heredoc_fork(int pipefd[2], t_heredoc_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		child_heredoc_process(ctx, pipefd[1]);
	}
	close(pipefd[1]);
	return (pid);
}

int	handle_heredoc_result(int pipefd[2], pid_t pid, t_env *env)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		env->last_status = 130;
		write(1, "\n", 1);
		return (-2);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipefd[0]);
		env->last_status = WEXITSTATUS(status);
		write(1, "\n", 1);
		return (-1);
	}
	env->last_status = 0;
	return (pipefd[0]);
}

int	handle_heredoc(char *raw_delimiter, bool expand, t_env *env, t_gc *gc)
{
	int				pipefd[2];
	pid_t			pid;
	char			*delimiter;
	t_heredoc_ctx	ctx;

	if (is_quoted(raw_delimiter))
	{
		delimiter = remove_quotes(gc, raw_delimiter);
		expand = false;
	}
	else
		delimiter = gc_strdup(gc, raw_delimiter);
	if (!delimiter || pipe(pipefd) == -1)
		return (-1);
	ctx.delimiter = delimiter;
	ctx.expand = expand;
	ctx.env = env;
	ctx.gc = gc;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = handle_heredoc_fork(pipefd, &ctx);
	if (pid == -1)
		return (-1);
	return (handle_heredoc_result(pipefd, pid, env));
}
