/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:09 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/15 23:02:11 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_env_var(t_env *env, char *var, t_gc *gc)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (env->env && env->env[count])
		count++;
	new_env = gc_malloc(gc, sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = env->env[i];
		i++;
	}
	new_env[count] = gc_strdup(gc, var);
	if (!new_env[count])
		return (1);
	new_env[count + 1] = NULL;
	env->env = new_env;
	return (0);
}

int	builtin_env(t_env *env)
{
	int	i;

	i = 0;
	while (env->env && env->env[i])
	{
		printf("%s\n", env->env[i]);
		i++;
	}
	return (0);
}
