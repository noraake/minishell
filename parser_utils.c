/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:08 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 13:34:02 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_operator(char c)
{
	return (c == '<' || c == '>');
}

int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int	count_clean_args(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (is_redirection_string(args[i]))
		{
			if (args[i + 1])
				i += 2;
			else
				i++;
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static void	fill_clean_args(char **args, char **clean_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redirection_string(args[i]))
		{
			if (args[i + 1])
				i += 2;
			else
				i++;
		}
		else
		{
			clean_args[j++] = args[i];
			i++;
		}
	}
	clean_args[j] = NULL;
}

char	**clean_args_after_redirections(char **args, t_gc *gc)
{
	int		count;
	char	**clean_args;

	count = count_clean_args(args);
	clean_args = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!clean_args)
		return (NULL);
	fill_clean_args(args, clean_args);
	return (clean_args);
}
