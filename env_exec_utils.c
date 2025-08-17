/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:23 by noakebli          #+#    #+#             */
/*   Updated: 2025/07/17 11:49:13 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_array(t_env *env, t_gc *gc)
{
	int		i;
	char	**arr;

	if (!env || !env->env)
		return (NULL);
	i = 0;
	while (env->env[i])
		i++;
	arr = gc_malloc(gc, sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env->env[i])
	{
		arr[i] = gc_strdup(gc, env->env[i]);
		if (!arr[i])
		{
			return (NULL);
		}
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
