/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:35 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/06 03:02:19 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_current_arg(const char *s, int *i, char *quote)
{
	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '"') && *quote == 0)
		{
			*quote = s[*i];
			(*i)++;
		}
		else if (s[*i] == *quote)
		{
			*quote = 0;
			(*i)++;
		}
		else if (is_space(s[*i]) && *quote == 0)
			break ;
		else
			(*i)++;
	}
}

static int	count_args(const char *s)
{
	int		count;
	int		i;
	char	quote;

	count = 0;
	i = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			break ;
		count++;
		skip_current_arg(s, &i, &quote);
	}
	return (count);
}

static void	free_args_array(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (args[i])
			free(args[i]);
		i++;
	}
	free(args);
}

static char	*extract_argument(char *s, int *i)
{
	int		start;
	char	quote;

	start = *i;
	quote = 0;
	while (s[*i])
	{
		if ((s[*i] == '\'' || s[*i] == '"') && quote == 0)
		{
			quote = s[*i];
			(*i)++;
		}
		else if (s[*i] == quote)
		{
			quote = 0;
			(*i)++;
		}
		else if (is_space(s[*i]) && quote == 0)
			break ;
		else
			(*i)++;
	}
	return (ft_strndup(&s[start], *i - start));
}

char	**split_arguments(char *s)
{
	int		i;
	int		j;
	int		arg_count;
	char	**args;

	i = 0;
	j = 0;
	arg_count = count_args(s);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			break ;
		args[j] = extract_argument(s, &i);
		if (!args[j])
			return (free_args_array(args, j), NULL);
		j++;
	}
	args[j] = NULL;
	return (args);
}
