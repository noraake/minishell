/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:50:57 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/06 23:05:24 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection_validation(char *line, int *i)
{
	char	current;
	int		count;

	current = line[*i];
	count = 1;
	if (line[*i + 1] == current)
	{
		count++;
		(*i)++;
	}
	(*i)++;
	while (line[*i] && is_space(line[*i]))
		(*i)++;
	if (!line[*i] || is_redirect(line[*i]))
		return (1);
	return (0);
}

int	process_character(char *line, int *i, char *quote,
	bool *first_non_space)
{
	if ((line[*i] == '"' || line[*i] == '\'') && *quote == 0)
	{
		*quote = line[*i];
		*first_non_space = false;
	}
	else if (line[*i] == *quote)
		*quote = 0;
	else if (is_pipe_char(line[*i]) && *quote == 0)
	{
		if (handle_pipe_validation(line, i, first_non_space) == 1)
			return (1);
		return (2);
	}
	else if (!is_space(line[*i]))
		*first_non_space = false;
	return (0);
}

int	check_invalid_pipes(char *line)
{
	int		i;
	char	quote;
	bool	first_non_space;
	int		result;

	i = 0;
	quote = 0;
	first_non_space = true;
	while (line[i] && is_space(line[i]))
		i++;
	while (line[i])
	{
		result = process_character(line, &i, &quote, &first_non_space);
		if (result == 1)
			return (1);
		if (result == 2)
		{
			i++;
			continue ;
		}
		i++;
	}
	return (0);
}
