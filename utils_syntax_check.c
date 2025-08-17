/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_syntax_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:48 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/16 23:46:09 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	is_redirect(char c)
{
	return (c == '<' || c == '>');
}

char	**list_to_array(t_list *tokens, t_gc *gc)
{
	int		len;
	char	**arr;
	int		i;

	len = ft_lstsize(tokens);
	arr = gc_malloc(gc, sizeof(char *) * (len + 1));
	i = 0;
	while (tokens)
	{
		arr[i] = gc_strdup(gc, tokens->content);
		i++;
		tokens = tokens->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
