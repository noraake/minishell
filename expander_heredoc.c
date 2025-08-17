/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 21:03:08 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/12 21:03:55 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_char_in_heredoc_expand(t_expander_ctx *ctx)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (ctx->str[ctx->i])
	{
		if (ctx->str[ctx->i] == '\'' && !in_double)
		{
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
			in_single = !in_single;
		}
		else if (ctx->str[ctx->i] == '"' && !in_single)
		{
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
			in_double = !in_double;
		}
		else if (ctx->str[ctx->i] == '$')
			handle_variable(ctx);
		else
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
	}
}

char	*expand_variables_heredoc(char *str, t_env *env, t_gc *gc)
{
	char			*result;
	t_expander_ctx	ctx;

	if (!str)
		return (NULL);
	result = gc_malloc(gc, ft_strlen(str) * 2 + 1);
	if (!result)
		return (NULL);
	ctx.str = str;
	ctx.i = 0;
	ctx.result = result;
	ctx.j = 0;
	ctx.env = env;
	process_char_in_heredoc_expand(&ctx);
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
