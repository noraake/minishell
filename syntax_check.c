/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:45 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/09 20:57:23 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes_closed(char *line)
{
	int		i;
	char	quote;
	int		len;

	if (!line)
		return (1);
	i = 0;
	quote = 0;
	len = ft_strlen(line);
	while (i < len && line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}

int	handle_pipe_validation(char *line, int *i, bool *first_non_space)
{
	if (*first_non_space)
		return (1);
	(*i)++;
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	if (!line[*i] || is_pipe_char(line[*i]))
		return (1);
	return (0);
}

int	check_invalid_redirections(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		else if (is_redirect(line[i]) && quote == 0)
		{
			if (handle_redirection_validation(line, &i) == 1)
				return (1);
			continue ;
		}
		i++;
	}
	return (0);
}

int	check_syntax_errors(char *line)
{
	if (check_quotes_closed(line))
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (1);
	}
	if (check_invalid_pipes(line))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	if (check_invalid_redirections(line))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (1);
	}
	return (0);
}
