/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:21:27 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 01:40:01 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(char c)
{
	return (c == '<' || c == '>');
}

void	init_token_ctx(t_token_ctx *ctx, t_list **tokens, const char *line,
		t_gc *gc)
{
	ctx->tokens = tokens;
	ctx->line = line;
	ctx->gc = gc;
}

void	add_token(t_token_ctx *ctx)
{
	char	*substr;

	if (ctx->start < ctx->end)
	{
		substr = gc_substr(ctx->gc, ctx->line, ctx->start, ctx->end
				- ctx->start);
		if (substr && *substr)
			ft_lstadd_back(ctx->tokens, ft_lstnew_gc(ctx->gc, substr));
	}
}
