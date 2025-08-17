/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:47:06 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 13:44:41 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_quote_in_pipe(t_pipe_split_ctx *ctx)
{
	if (is_quote(ctx->line[ctx->i]))
	{
		if (*ctx->quote == 0)
			*ctx->quote = ctx->line[ctx->i];
		else if (*ctx->quote == ctx->line[ctx->i])
			*ctx->quote = 0;
	}
}

static void	handle_pipe_char(t_pipe_split_ctx *ctx)
{
	if (ctx->line[ctx->i] == '|' && *ctx->quote == 0)
	{
		ft_lstadd_back(ctx->pieces, ft_lstnew_gc(ctx->gc,
				gc_substr(ctx->gc, ctx->line, *ctx->start,
					ctx->i - *ctx->start)));
		*ctx->start = ctx->i + 1;
	}
}

void	add_pipe_if_needed(t_pipe_split_ctx *ctx)
{
	handle_quote_in_pipe(ctx);
	handle_pipe_char(ctx);
}
