/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:11 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 13:34:14 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe(int *fds)
{
	if (pipe(fds) == -1)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

int	is_pipe_char(char c)
{
	return (c == '|');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_list	*ft_lstnew_gc(t_gc *gc, void *content)
{
	t_list	*node;

	node = gc_malloc(gc, sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
