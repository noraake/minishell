/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aaloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 01:39:03 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/16 23:39:37 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_destroy(t_gc *gc)
{
	if (!gc)
		return ;
	gc_free_all(gc);
	free(gc);
}

void	*gc_malloc(t_gc *gc, size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	if (!gc || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	memset(ptr, 0, size);
	node->ptr = ptr;
	node->next = gc->head;
	gc->head = node;
	return (ptr);
}

char	*gc_strdup(t_gc *gc, const char *src)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(src);
	dup = gc_malloc(gc, len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, src);
	return (dup);
}

void	gc_free_all(t_gc *gc)
{
	t_gc_node	*curr;
	t_gc_node	*next;

	if (!gc)
		return ;
	curr = gc->head;
	while (curr)
	{
		next = curr->next;
		free(curr->ptr);
		free(curr);
		curr = next;
	}
	gc->head = NULL;
}

void	gc_clear_command_memory(t_gc *gc)
{
	if (!gc)
		return ;
	gc_free_all(gc);
}
