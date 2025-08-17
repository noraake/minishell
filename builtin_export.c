/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 02:30:55 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/17 01:10:08 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_name(const char *name)
{
	size_t	i;

	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (false);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

char	*normalize_escaped_name(const char *name, t_gc *gc)
{
	size_t	len;
	char	*res;
	size_t	i;
	size_t	j;

	len = ft_strlen(name);
	res = gc_malloc(gc, len + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (name[i])
	{
		if (name[i] == '\\' && name[i + 1] && (ft_isalnum(name[i + 1])
				|| name[i + 1] == '_'))
		{
			res[j++] = name[i + 1];
			i += 2;
		}
		else
			res[j++] = name[i++];
	}
	res[j] = '\0';
	return (res);
}

char	*make_assignment(const char *name, const char *value)
{
	char	*eq;
	char	*full;

	eq = ft_strjoin(name, "=");
	if (!eq)
		return (NULL);
	full = ft_strjoin(eq, value);
	free(eq);
	if (!full)
		return (NULL);
	return (full);
}

int	print_exported_vars(t_env *env)
{
	int		i;
	char	*eq;

	i = 0;
	while (env->env && env->env[i])
	{
		eq = ft_strchr(env->env[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("declare -x %s=\"%s\"\n", env->env[i], eq + 1);
			*eq = '=';
		}
		else
			printf("declare -x %s\n", env->env[i]);
		i++;
	}
	i = 0;
	while (env->export_only && env->export_only[i])
		printf("declare -x %s\n", env->export_only[i++]);
	return (0);
}

int	builtin_export(char **args, t_env *env, t_gc *gc)
{
	int	i;
	int	exit_code;

	exit_code = 0;
	if (!args[1])
		return (print_exported_vars(env));
	if (args[1][0] == '-' && args[1][1] != '\0')
	{
		ft_putstr_fd("export: invalid option\n", 2);
		return (2);
	}
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(args[i], env, gc) != 0)
			exit_code = 1;
		i++;
	}
	return (exit_code);
}
