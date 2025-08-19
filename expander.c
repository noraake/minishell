/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:41 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/18 17:44:06 by noakebli         ###   ########.fr       */
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
	size_t	allocated_size;

	in_single = false;
	in_double = false;
	allocated_size = calculate_expanded_size(ctx->str, ctx->env);
	while (ctx->str[ctx->i])
	{
		if (update_quote_state(ctx->str[ctx->i], &in_single, &in_double))
			ctx->i++;
		else if (ctx->str[ctx->i] == '$' && !in_single)
			handle_variable(ctx);
		else
		{
			if (ctx->j >= allocated_size - 1)
				break ;
			ctx->result[ctx->j++] = ctx->str[ctx->i++];
		}
	}
	ctx->result[ctx->j] = '\0';
}

char	*expand_variables(char *str, t_env *env, t_gc *gc)
{
	char			*result;
	t_expander_ctx	ctx;
	size_t			size;

	if (!str)
		return (NULL);
	if (!*str)
		return (gc_strdup(gc, ""));
	size = calculate_expanded_size(str, env);
	if (size < ft_strlen(str) + 256)
		size = ft_strlen(str) + 256;
	result = gc_malloc(gc, size);
	if (!result)
		return (NULL);
	ctx.str = str;
	ctx.i = 0;
	ctx.j = 0;
	ctx.result = result;
	ctx.env = env;
	process_char_in_expand_no_quotes(&ctx);
	ctx.result[ctx.j] = '\0';
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
