/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_structs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:33 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/13 00:55:28 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_cmd(char *cmd, char **args, t_gc *gc)
{
	t_cmd	*new;

	new = gc_malloc(gc, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd_path = cmd;
	new->args = args;
	new->infile = NULL;
	new->outfile = NULL;
	new->append = 0;
	new->next = NULL;
	return (new);
}

void	init_pipe_context(t_pipe_context *ctx, t_cmd *cmds, t_env *env,
		t_gc *gc)
{
	int	count;

	count = count_cmds(cmds);
	ctx->cmd = cmds;
	ctx->env = env;
	ctx->gc = gc;
	ctx->pids = gc_malloc(gc, sizeof(pid_t) * count);
	ctx->current = cmds;
	ctx->prev_fd = -1;
	ctx->cmd_index = 0;
}
