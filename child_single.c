/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_single.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 02:55:24 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 09:58:55 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prepare_and_handle_special_cases(t_cmd *cmds, t_env *env, t_gc *gc)
{
	int	status;

	setup_exec_signals();
	handle_redirections_new(cmds, env, gc);
	if (is_builtin(cmds->args[0]))
	{
		status = exec_builtin(cmds, env, gc, true);
		cleanup_and_exit(gc, status);
	}
	if (!ft_strcmp(cmds->args[0], ".") || !ft_strcmp(cmds->args[0], ".."))
	{
		print_cmd_not_found(cmds->args[0]);
		cleanup_and_exit(gc, 127);
	}
}

static void	execute_command(t_cmd *cmds, t_env *env, t_gc *gc)
{
	char	*path;

	if (strchr(cmds->args[0], '/'))
		exec_with_checks_exit(cmds->args[0], cmds, env);
	else
	{
		path = find_command_path(cmds->args[0], env, gc);
		if (!path)
		{
			print_cmd_not_found(cmds->args[0]);
			cleanup_and_exit(gc, 127);
		}
		exec_with_checks_exit(path, cmds, env);
	}
}

void	child_exec_single(t_cmd *cmds, t_env *env, t_gc *gc)
{
	if (!cmds || !cmds->args || !cmds->args[0] || !*(cmds->args[0]))
		cleanup_and_exit(gc, 0);
	prepare_and_handle_special_cases(cmds, env, gc);
	execute_command(cmds, env, gc);
}
