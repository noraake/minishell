/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:56:32 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 01:22:02 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_export_name_and_value(char *arg, char **name, char **value)
{
	char	*equal;
	size_t	name_len;

	*name = NULL;
	*value = NULL;
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		name_len = equal - arg;
		*name = ft_substr(arg, 0, name_len);
		*value = ft_strdup(equal + 1);
		return (0);
	}
	*name = ft_strdup(arg);
	return (0);
}

int	validate_and_normalize_name(char **name, char *value, char *arg, t_gc *gc)
{
	char	*normalized;

	if (!*name)
		return (1);
	normalized = normalize_escaped_name(*name, gc);
	free(*name);
	if (!normalized)
	{
		free(value);
		return (1);
	}
	*name = normalized;
	if (!is_valid_name(*name))
	{
		free(value);
		return (print_error_invalid(arg));
	}
	return (0);
}

int	handle_export_with_value(char *name, char *value, t_env *env, t_gc *gc)
{
	char	*assignment;

	assignment = make_assignment(name, value);
	if (!assignment)
		return (1);
	add_or_update_env_var(env, assignment, gc);
	free(assignment);	
	return (0);
}

int	handle_export_without_value(char *name, t_env *env, t_gc *gc)
{
	if (get_env_value(env, name))
		return (0);
	return (add_to_export_only(env, name, gc));
}

int	handle_export_arg(char *arg, t_env *env, t_gc *gc)
{
	char	*name;
	char	*value;
	int		ret;

	value = NULL;
	extract_export_name_and_value(arg, &name, &value);
	ret = validate_and_normalize_name(&name, value, arg, gc);
	if (ret != 0)
		return (ret);
	if (value)
		ret = handle_export_with_value(name, value, env, gc);
	else
		ret = handle_export_without_value(name, env, gc);
	free(value);
	return (ret);
}
