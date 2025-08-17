/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 12:13:25 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/16 16:38:37 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_external_cmd(t_pipe_context *ctx)
{
	char	*path;

	path = find_command_path(ctx->current->args[0], ctx->env, ctx->gc);
	if (!path)
	{
		print_cmd_not_found(ctx->current->args[0]);
		cleanup_and_exit(ctx->gc, 127);
	}
	exec_with_checks_exit(path, ctx->current, ctx->env);
}

void	update_status(int status, t_env *env)
{
	int	sig;

	if (WIFEXITED(status))
		env->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			env->last_status = 130;
		else if (sig == SIGQUIT)
		{
			env->last_status = 131;
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		}
		else
			env->last_status = 128 + sig;
	}
	else
		env->last_status = 1;
}

void	exec_child_process(t_pipe_context *ctx, int pipefd[2])
{
	setup_exec_signals();
	if (ctx->prev_fd != -1)
	{
		if (dup2(ctx->prev_fd, STDIN_FILENO) < 0)
			perror("dup2 prev_fd");
		close_fd_if_valid(ctx->prev_fd);
	}
	if (ctx->current->next && pipefd[1] != -1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			perror("dup2 pipefd[1]");
	}
	close_fd_if_valid(pipefd[0]);
	close_fd_if_valid(pipefd[1]);
	handle_command_execution(ctx);
}

void	manage_fds_after_fork(t_pipe_context *ctx, int pipefd[2])
{
	close_fd_if_valid(ctx->prev_fd);
	if (ctx->current->next)
	{
		ctx->prev_fd = pipefd[0];
		close_fd_if_valid(pipefd[1]);
	}
	else
	{
		close_fd_if_valid(pipefd[0]);
		close_fd_if_valid(pipefd[1]);
		ctx->prev_fd = -1;
	}
}
