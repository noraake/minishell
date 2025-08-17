/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spliiit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:27:22 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/07 01:40:24 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_split_ctx(t_split_ctx *ctx, const char *line, t_gc *gc,
		t_list **tokens)
{
	ctx->line = line;
	ctx->gc = gc;
	ctx->tokens = tokens;
	ctx->i = 0;
	ctx->start = 0;
	ctx->quote = 0;
	ctx->len = ft_strlen(line);
}

static void	process_redirection(t_split_ctx *ctx)
{
	t_token_ctx	tok_ctx;

	init_token_ctx(&tok_ctx, ctx->tokens, ctx->line, ctx->gc);
	tok_ctx.start = ctx->start;
	tok_ctx.end = ctx->i;
	add_token(&tok_ctx);
	ctx->i += handle_redirection(ctx->line, ctx->i, ctx->tokens, ctx->gc);
	while (ctx->line[ctx->i] && (ctx->line[ctx->i] == ' '
			|| ctx->line[ctx->i] == '\t'))
		(ctx->i)++;
	ctx->start = ctx->i;
}

static void	handle_spaces(t_split_ctx *ctx)
{
	t_token_ctx	tok_ctx;

	init_token_ctx(&tok_ctx, ctx->tokens, ctx->line, ctx->gc);
	tok_ctx.start = ctx->start;
	tok_ctx.end = ctx->i;
	add_token(&tok_ctx);
	while (ctx->line[ctx->i] && (ctx->line[ctx->i] == ' '
			|| ctx->line[ctx->i] == '\t'))
		(ctx->i)++;
	ctx->start = ctx->i;
}

char	**split_operators(const char *line, t_gc *gc)
{
	t_split_ctx	ctx;
	t_list		*tokens;
	t_token_ctx	tok_ctx;

	tokens = NULL;
	init_split_ctx(&ctx, line, gc, &tokens);
	while (ctx.i < ctx.len && line[ctx.i])
	{
		handle_quote(line[ctx.i], &ctx.quote);
		if (is_redir(line[ctx.i]) && ctx.quote == 0)
			process_redirection(&ctx);
		else if ((line[ctx.i] == ' ' || line[ctx.i] == '\t') && ctx.quote == 0)
			handle_spaces(&ctx);
		else
			ctx.i++;
	}
	init_token_ctx(&tok_ctx, &tokens, line, gc);
	tok_ctx.start = ctx.start;
	tok_ctx.end = ctx.i;
	add_token(&tok_ctx);
	return (list_to_array(tokens, gc));
}
