/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:58:13 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/06 21:58:13 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *arg)
{
	return (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">")
		|| !ft_strcmp(arg, ">>") || !ft_strcmp(arg, "<<"));
}

bool	is_quoted(char *str)
{
	if (!str || ft_strlen(str) < 2)
		return (false);
	return ((str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		|| (str[0] == '"' && str[ft_strlen(str) - 1] == '"'));
}

int	print_redirection_error(char *msg)
{
	ft_putstr_fd("minishell: erreur de redirection", 2);
	if (msg)
	{
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(msg, 2);
	}
	else
		ft_putchar_fd('\n', 2);
	return (1);
}

bool	has_command(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!is_redirection(args[i])
			&& (!args[i + 1] || !is_redirection(args[i + 1])))
			return (true);
		i++;
	}
	return (false);
}
