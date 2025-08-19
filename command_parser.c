/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:18 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 10:07:08 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_invalid_args_after_redirection(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (is_redirection_string(args[i]))
		{
			if (!args[i + 1] || is_redirection_string(args[i + 1]))
				return (true);
			i += 2;
		}
		else
			i++;
	}
	return (false);
}

static char	**get_args_and_alloc_was_quoted(const char *command_str, t_gc *gc,
		bool **was_quoted_out)
{
	char	**args;
	int		argc;

	args = split_operators(command_str, gc);
	if (!args)
		return (NULL);
	argc = 0;
	while (args[argc])
		argc++;
	*was_quoted_out = gc_malloc(gc, sizeof(bool) * (argc + 1));
	if (!*was_quoted_out)
	{
		return (NULL);
	}
	return (args);
}

static	t_cmd	*create_and_setup_cmd(char **args,
					bool **was_quoted_out, t_gc *gc)
{
	t_cmd	*cmd;

	cmd = gc_malloc(gc, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->next = NULL;
	cmd->redirections = NULL;
	if (!args[0])
	{
		cmd->args = args;
		cmd->was_empty_unquoted = true;
		return (cmd);
	}
	if (!*(args[0]) && (*was_quoted_out)[0])
	{
		cmd->was_empty_quoted = true;
	}
	cmd->args = args;
	return (cmd);
}

t_cmd	*init_cmd_and_parse_args(const char *command_str, t_env *env, t_gc *gc,
			bool **was_quoted_out)
{
	char		**args;
	t_context	ctx;

	if (!command_str)
		return (NULL);
	args = get_args_and_alloc_was_quoted(command_str, gc, was_quoted_out);
	if (!args)
		return (NULL);
	ctx.env = env;
	ctx.gc = gc;
	if (!expand_and_clean_args(args, &ctx, *was_quoted_out))
		return (NULL);
	return (create_and_setup_cmd(args, was_quoted_out, gc));
}

t_cmd	*parse_all_commands_gc(char **command_strings, t_env *env, t_gc *gc)
{
	int		i;
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;

	i = 0;
	head = NULL;
	last = NULL;
	if (!command_strings)
		return (NULL);
	while (command_strings[i])
	{
		cmd = parse_command(command_strings[i++], env, gc);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
	}
	return (head);
}
