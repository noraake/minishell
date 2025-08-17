/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:15 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 14:07:44 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_index
{
	int	i;
	int	j;
}	t_index;

int	validate_quotes(const char *str)
{
	int		i;
	char	quote;
	int		len;

	i = 0;
	quote = 0;
	len = ft_strlen(str);
	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}

static void	process_quoted_char(const char *str, char *result,
	t_index *idx, char *quote)
{
	if (!*quote)
		*quote = str[(idx->i)++];
	else if (str[idx->i] == *quote)
	{
		*quote = 0;
		(idx->i)++;
	}
	else
		result[(idx->j)++] = str[(idx->i)++];
}

static void	process_string(const char *str, char *result)
{
	t_index	idx;
	char	quote;

	idx.i = 0;
	idx.j = 0;
	quote = 0;
	while (str[idx.i])
	{
		if (str[idx.i] == '\'' || str[idx.i] == '"')
			process_quoted_char(str, result, &idx, &quote);
		else
			result[idx.j++] = str[idx.i++];
	}
	result[idx.j] = '\0';
}

char	*remove_quotes(t_gc *gc, const char *str)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = gc_malloc(gc, len + 1);
	if (!result)
		return (NULL);
	process_string(str, result);
	return (result);
}
