/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_pars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 03:00:38 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/08 03:12:35 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*handle_empty_args(t_cmd *cmd, t_gc *gc)
{
	if (!cmd->args || !cmd->args[0])
	{
		cmd->args = gc_malloc(gc, sizeof(char *));
		if (!cmd->args)
			return (NULL);
		cmd->args[0] = NULL;
		cmd->redirections = NULL;
		return (cmd);
	}
	return (cmd);
}

t_cmd	*process_redirections(t_cmd *cmd, bool *was_quoted, t_env *env,
		t_gc *gc)
{
	char		**clean_args;
	int			error;
	t_redir_ctx	ctx;

	error = 0;
	if (has_invalid_args_after_redirection(cmd->args))
	{
		ft_putstr_fd("minishell: ambiguous or invalid redirection\n", 2);
		return (NULL);
	}
	ctx.args = cmd->args;
	ctx.was_quoted = was_quoted;
	ctx.env = env;
	ctx.error = &error;
	ctx.gc = gc;
	cmd->redirections = parse_redirections(&ctx);
	if (error)
		return (NULL);
	clean_args = clean_args_after_redirections(cmd->args, gc);
	cmd->args = clean_args;
	return (cmd);
}

t_cmd	*parse_command(const char *command_str, t_env *env, t_gc *gc)
{
	t_cmd	*cmd;
	bool	*was_quoted;

	cmd = init_cmd_and_parse_args(command_str, env, gc, &was_quoted);
	if (!cmd)
		return (NULL);
	cmd = handle_empty_args(cmd, gc);
	if (!cmd)
		return (NULL);
	if (!cmd->args || !cmd->args[0])
		return (cmd);
	return (process_redirections(cmd, was_quoted, env, gc));
}
