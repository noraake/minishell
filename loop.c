/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:57 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/18 17:34:48 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*redir_type_to_str(int type)
{
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == APPEND)
		return ("APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

static bool	line_is_empty_or_spaces(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (true);
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (false);
		i++;
	}
	return (true);
}

static int	handle_simple_assignment(char *line, t_env *env)
{
	handle_assignment(line, env, env->gc);
	env->last_status = 0;
	return (0);
}

int	process_input(char *line, t_env *env)
{
	char	**commands;
	t_cmd	*cmd_table;
	char	*eq;

	if (line_is_empty_or_spaces(line))
		return (0);
	eq = ft_strchr(line, '=');
	if (eq && !ft_strchr(line, ' ') && is_assignment(line))
		return (handle_simple_assignment(line, env));
	commands = split_by_pipe_gc(line, env->gc);
	if (!commands)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		env->last_status = 2;
		return (0);
	}
	cmd_table = parse_all_commands_gc(commands, env, env->gc);
	if (!cmd_table)
	{
		env->last_status = 2;
		return (0);
	}
	execute_pipeline(cmd_table, env, env->gc);
	return (0);
}
