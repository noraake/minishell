/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:26 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 03:20:35 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *var)
{
	int	i;
	int	len;

	if (!env || !env->env || !var)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (env->env[i])
	{
		if (ft_strncmp(env->env[i], var, len) == 0 && env->env[i][len] == '=')
			return (env->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static void	remove_from_table(char **table, char *key)
{
	int		i;
	int		j;
	size_t	len;

	if (!table || !key)
		return ;
	len = ft_strlen(key);
	i = 0;
	while (table[i])
	{
		if (!ft_strncmp(table[i], key, len)
			&& (table[i][len] == '=' || table[i][len] == '\0'))
		{
			j = i;
			while (table[j + 1])
			{
				table[j] = table[j + 1];
				j++;
			}
			table[j] = NULL;
			break ;
		}
		i++;
	}
}

int	remove_env_var(t_env *env, char *key)
{
	if (!env || !key)
		return (1);
	remove_from_table(env->env, key);
	remove_from_table(env->export_only, key);
	return (0);
}

int	add_or_update_env_var(t_env *env, char *var, t_gc *gc)
{
	char	*key;
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	while (var[len] && var[len] != '=')
		len++;
	key = ft_strndup_gc(var, len, gc);
	if (!key)
		return (1);
	while (env->env && env->env[i])
	{
		if (!ft_strncmp(env->env[i], key, len) && env->env[i][len] == '=')
		{
			env->env[i] = gc_strdup(gc, var);
			return (env->env[i] == NULL);
		}
		i++;
	}
	return (append_env_var(env, var, gc));
}
