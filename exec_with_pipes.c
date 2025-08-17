/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:15:07 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/16 16:39:59 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_execution(t_pipe_context *ctx)
{
	int	status;

	handle_redirections_new(ctx->current, ctx->env, ctx->gc);
	if (!ctx->current->args || !ctx->current->args[0]
		|| !*(ctx->current->args[0]))
	{
		collapse_empty_args(ctx->current->args);
		cleanup_and_exit(ctx->gc, 0);
	}
	if (is_builtin(ctx->current->args[0]))
	{
		status = exec_builtin(ctx->current, ctx->env, ctx->gc);
		cleanup_and_exit(ctx->gc, status);
	}
	if (strchr(ctx->current->args[0], '/'))
		exec_with_checks_exit(ctx->current->args[0], ctx->current, ctx->env);
	else
		exec_external_cmd(ctx);
}

static void	execute_all(t_pipe_context *ctx)
{
	int	pipefd[2];

	while (ctx->current)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		setup_and_fork(ctx, pipefd);
		manage_fds_after_fork(ctx, pipefd);
		ctx->current = ctx->current->next;
		ctx->cmd_index++;
	}
	close_fd_if_valid(ctx->prev_fd);
}

static void	update_status_from_wait(int status, t_env *env, int *printed_sigint)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT && !*printed_sigint)
	{
		write(STDOUT_FILENO, "\n", 1);
		*printed_sigint = 1;
		env->last_status = 130;
	}
	else if (WIFEXITED(status))
		env->last_status = WEXITSTATUS(status);
}

static void	wait_all_pipes(pid_t *pids, int count, t_env *env)
{
	int			i;
	int			status;
	int			last_status;
	static int	printed_sigint = 0;
	pid_t		shell_pgid;

	i = -1;
	last_status = 0;
	printed_sigint = 0;
	while (++i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid");
		else
			update_status_from_wait(status, env, &printed_sigint);
		if (i == count - 1)
			last_status = status;
	}
	if (isatty(STDIN_FILENO))
	{
		shell_pgid = getpid();
		if (ioctl(STDIN_FILENO, TIOCSPGRP, &shell_pgid) == -1)
			perror("ioctl TIOCSPGRP");
	}
	update_status(last_status, env);
}

void	exec_with_pipes(t_cmd *cmds, t_env *env, t_gc *gc)
{
	t_pipe_context	ctx;
	int				count;

	count = count_cmds(cmds);
	if (count == 0)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	init_pipe_context(&ctx, cmds, env, gc);
	execute_all(&ctx);
	wait_all_pipes(ctx.pids, count, env);
	setup_signals();
}
