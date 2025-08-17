/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 04:32:18 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/15 23:04:21 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_arg(char **args, bool *was_quoted, int i, t_context *ctx)
{
	char	*final_or_expanded;

	if (i > 0 && args[i - 1] && !ft_strcmp(args[i - 1], "<<"))
	{
		was_quoted[i] = has_quotes(args[i]);
		final_or_expanded = remove_quotes(ctx->gc, args[i]);
	}
	else
	{
		was_quoted[i] = has_quotes(args[i]);
		final_or_expanded = expand_variables(args[i], ctx->env, ctx->gc);
	}
	if (!final_or_expanded)
		return (0);
	args[i] = final_or_expanded;
	return (1);
}

static void	compact_args_array(char **args, bool *was_quoted)
{
	int	i;
	int	j;

	i = 0;
	while (args[i] && (!args[i] || !args[i][0]))
		i++;
	if (i > 0 && args[i])
	{
		j = 0;
		while (args[i])
		{
			args[j] = args[i];
			was_quoted[j] = was_quoted[i];
			i++;
			j++;
		}
		args[j] = NULL;
	}
}

int	expand_and_clean_args(char **args, t_context *ctx, bool *was_quoted)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (!process_arg(args, was_quoted, i, ctx))
			return (0);
		i++;
	}
	compact_args_array(args, was_quoted);
	return (1);
}
