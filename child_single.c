/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 02:55:24 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/16 16:53:14 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_exec_single(t_cmd *cmds, t_env *env, t_gc *gc)
{
	char	*path;

	setup_exec_signals();
	handle_redirections_new(cmds, env, gc);
	if (!cmds || !cmds->args || !cmds->args[0] || !*(cmds->args[0]))
		cleanup_and_exit(gc, 0);
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
