/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:37 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 22:11:09 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	collapse_empty_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!args)
		return ;
	while (args[i] && args[i][0] == '\0')
		i++;
	if (i == 0)
		return ;
	while (args[i])
		args[j++] = args[i++];
	args[j] = NULL;
}

void	handle_redirection_exit_status(int status, t_env *env)
{
	if (WIFEXITED(status))
		env->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_signaled_status(status, env);
	else
		env->last_status = 1;
}

void	handle_redirection_only(t_cmd *cmds, t_env *env, t_gc *gc)
{
	pid_t	pid;
	int		status;

	set_signals_for_redirection();
	pid = fork();
	if (pid == 0)
	{
		handle_redirections_new(cmds, env, gc);
		cleanup_and_exit(gc, 0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		handle_redirection_exit_status(status, env);
	}
	else
	{
		perror("fork");
		env->last_status = 1;
	}
}

void	execute_pipeline(t_cmd *cmds, t_env *env, t_gc *gc)
{
	if (!cmds)
		return ;
	if (!cmds->next)
	{
		if (cmds->args && cmds->args[0] && is_builtin(cmds->args[0]))
			env->last_status = execute_builtin_with_redirection(cmds, env, gc);
		else
			execute_single_cmd(cmds, env, gc);
	}
	else
		exec_with_pipes(cmds, env, gc);
}
