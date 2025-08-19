/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:36:49 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/18 17:11:46 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_dollar_and_advance(char *str, int *i, bool in_single,
		t_env *env)
{
	if (str[*i] == '$' && !in_single)
	{
		if (str[*i + 1] == '\0')
		{
			(*i)++;
			return (1);
		}
		else
			return (process_var_size(str, i, env));
	}
	return (0);
}

int	calculate_expanded_size(char *str, t_env *env)
{
	int		i;
	int		size;
	bool	in_single;
	bool	in_double;

	i = 0;
	size = 0;
	in_single = false;
	in_double = false;
	while (str[i])
	{
		if (update_quote_state(str[i], &in_single, &in_double))
			i++;
		else
		{
			size += handle_dollar_and_advance(str, &i, in_single, env);
			if (str[i] && !(str[i] == '$' && !in_single))
			{
				size++;
				i++;
			}
		}
	}
	return (size + 1);
}
