/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 22:44:30 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/18 17:34:39 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_for_signals(t_env *new_env)
{
	static t_env	*env = NULL;

	if (new_env)
		env = new_env;
	return (env);
}

static int	handle_syntax_and_status(char *line, t_env *env)
{
	if (check_syntax_errors(line))
	{
		env->last_status = 2;
		return (0);
	}
	return (1);
}

static int	handle_process_status(int status, t_env *env)
{
	if (status == 130)
	{
		env->last_status = 130;
		return (0);
	}
	if (status == -1)
		return (-1);
	return (1);
}

void	loop_minishell(t_env *env)
{
	char	*line;
	int		status;

	get_env_for_signals(env);
	while (1)
	{
		setup_signals();
		line = read_input(env->gc);
		if (!line)
			break ;
		if (!*line)
			continue ;
		if (!handle_syntax_and_status(line, env))
			continue ;
		status = process_input(line, env);
		status = handle_process_status(status, env);
		if (status == -1)
			break ;
		if (status == 0)
			continue ;
	}
}
