/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:15:23 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 21:51:35 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signaled_status(int status, t_env *env)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGINT)
		env->last_status = 130;
	else if (sig == SIGQUIT)
		env->last_status = 131;
	else
		env->last_status = 128 + sig;
}

void	setup_and_fork(t_pipe_context *ctx, int pipefd[2])
{
	if (ctx->current->next && pipe(pipefd) < 0)
		perror("pipe");
	ctx->pids[ctx->cmd_index] = fork();
	if (ctx->pids[ctx->cmd_index] < 0)
	{
		perror("fork");
		return ;
	}
	if (ctx->pids[ctx->cmd_index] == 0)
		exec_child_process(ctx, pipefd);
}

void	wait_and_set_status(int status, t_env *env)
{
	int	sig;

	if (WIFEXITED(status))
		env->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			env->last_status = 130;
			write(STDOUT_FILENO, "\n", 1);
		}
		else if (sig == SIGQUIT)
		{
			env->last_status = 131;
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
		else
			env->last_status = 128 + sig;
	}
	else
		env->last_status = 1;
}

void	handle_exec(t_cmd *cmds, t_env *env, t_gc *gc)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		child_exec_single(cmds, env, gc);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		wait_and_set_status(status, env);
	}
	else
	{
		perror("fork");
		env->last_status = 1;
	}
}

void	execute_single_cmd(t_cmd *cmds, t_env *env, t_gc *gc)
{
	if (!cmds)
		return ;
	if (cmds->was_empty_unquoted || !cmds->args || !cmds->args[0]
		|| !*(cmds->args[0]))
	{
		if (cmds->redirections)
			handle_redirection_only(cmds, env, gc);
		else if (!cmds->was_empty_unquoted && cmds->args
			&& *(cmds->args[0]) == '\0')
		{
			printf("minishell: \"\": command not found\n");
			env->last_status = 127;
		}
		return ;
	}
	handle_exec(cmds, env, gc);
}
