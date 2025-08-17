/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:41:54 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 01:42:56 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quote(char c, char *quote)
{
	if ((*quote == 0) && (c == '\'' || c == '"'))
		*quote = c;
	else if (c == *quote)
		*quote = 0;
}

static void	add_redir_token(t_list **tokens, t_gc *gc, char *substr)
{
	if (substr)
		ft_lstadd_back(tokens, ft_lstnew_gc(gc, substr));
}

int	handle_redirection(const char *line, int i, t_list **tokens, t_gc *gc)
{
	int		len;
	char	*substr;

	len = ft_strlen(line);
	if ((line[i] == '>' && i + 1 < len && line[i + 1] == '>') || (line[i] == '<'
			&& i + 1 < len && line[i + 1] == '<'))
	{
		substr = gc_substr(gc, line, i, 2);
		add_redir_token(tokens, gc, substr);
		return (2);
	}
	substr = gc_substr(gc, line, i, 1);
	add_redir_token(tokens, gc, substr);
	return (1);
}
