/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:42 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 13:31:28 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_by_pipe_gc(char *line, t_gc *gc)
{
	t_pipe_split_ctx	ctx;
	t_list				*pieces;
	int					i;
	int					start;
	char				quote;

	pieces = NULL;
	i = 0;
	start = 0;
	quote = 0;
	while (line[i])
	{
		ctx.line = line;
		ctx.start = &start;
		ctx.i = i;
		ctx.quote = &quote;
		ctx.pieces = &pieces;
		ctx.gc = gc;
		add_pipe_if_needed(&ctx);
		i++;
	}
	ft_lstadd_back(&pieces, ft_lstnew_gc(gc,
			gc_substr(gc, line, start, i - start)));
	return (list_to_array(pieces, gc));
}
