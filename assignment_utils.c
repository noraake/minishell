/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 02:02:53 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/09 03:18:34 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_assignment(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (false);
	if (s[i] == '\\' && s[i + 1])
		i++;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '\\' && s[i + 1])
		{
			i += 2;
			continue ;
		}
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (s[i] == '=');
}

int	handle_assignment(const char *arg, t_env *env, t_gc *gc)
{
	if (add_or_update_env_var(env, (char *)arg, gc))
		return (1);
	return (0);
}
