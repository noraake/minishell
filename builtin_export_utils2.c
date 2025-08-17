/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 15:33:02 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/10 15:48:15 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_to_export_only(t_env *env, const char *name, t_gc *gc)
{
	int		j;
	int		i;
	char	**new_list;

	if (!env || !name)
		return (1);
	i = 0;
	if (env->export_only)
		while (env->export_only[i])
			if (ft_strcmp(env->export_only[i++], name) == 0)
				return (0);
	new_list = gc_malloc(gc, sizeof(char *) * (i + 2));
	if (!new_list)
		return (1);
	j = 0;
	while (j < i)
	{
		new_list[j] = env->export_only[j];
		j++;
	}
	new_list[i] = gc_strdup(gc, name);
	new_list[i + 1] = NULL;
	env->export_only = new_list;
	return (0);
}
