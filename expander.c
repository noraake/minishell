/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:41 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/12 21:03:04 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variables_keep_quotes(char *str, t_env *env, t_gc *gc)
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
	process_char_in_expand(&ctx);
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}

void	process_char_in_expand_no_quotes(t_expander_ctx *ctx)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (ctx->str[ctx->i])
	{
		if (update_quote_state(ctx->str[ctx->i], &in_single, &in_double))
			ctx->i++;
		else if (ctx->str[ctx->i] == '$' && !in_single)
			handle_variable(ctx);
		else
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
	}
}

char	*expand_variables(char *str, t_env *env, t_gc *gc)
{
	char			*result;
	t_expander_ctx	ctx;

	if (!str)
		return (NULL);
	if (!*str)
		return (gc_strdup(gc, ""));
	result = gc_malloc(gc, calculate_expanded_size(str, env));
	if (!result)
		return (NULL);
	ctx.str = str;
	ctx.i = 0;
	ctx.result = result;
	ctx.j = 0;
	ctx.env = env;
	process_char_in_expand_no_quotes(&ctx);
	ctx.result[ctx.j] = '\0';
	if (ctx.j == 0)
		return (gc_strdup(gc, ""));
	return (ctx.result);
}

static void	process_expand_with_quotes(t_expander_ctx *ctx)
{
	while (ctx->str[ctx->i])
	{
		if (ctx->str[ctx->i] == '$' && ctx->str[ctx->i + 1])
			handle_variable(ctx);
		else
			ctx->result[ctx->j++] = ctx->str[ctx->i]++;
	}
}

char	*expand_with_quotes(char *str, t_env *env, t_gc *gc)
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
	process_expand_with_quotes(&ctx);
	ctx.result[ctx.j] = '\0';
	return (ctx.result);
}
