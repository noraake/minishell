/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:38 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 20:42:08 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_path_args
{
	char	**paths;
	char	*src;
	int		j;
	t_gc	*gc;
}	t_path_args;

static int	count_path_elements(char *path_str)
{
	int	count;
	int	i;

	if (!path_str || !*path_str)
		return (0);
	count = 1;
	i = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
			count++;
		i++;
	}
	return (count);
}

static int	add_path_element(t_path_args *args, int start, int len)
{
	int	i;

	args->paths[args->j] = gc_malloc(args->gc, len + 1);
	if (!args->paths[args->j])
		return (0);
	i = 0;
	while (i < len)
	{
		args->paths[args->j][i] = args->src[start + i];
		i++;
	}
	args->paths[args->j][len] = '\0';
	args->j++;
	return (1);
}

static char	**process_path_elements(char **paths, char *path_str, t_gc *gc)
{
	t_path_args	args;
	int			i;
	int			start;

	args.paths = paths;
	args.src = path_str;
	args.j = 0;
	args.gc = gc;
	i = 0;
	start = 0;
	while (path_str[i])
	{
		if (path_str[i] == ':')
		{
			if (!add_path_element(&args, start, i - start))
				return (NULL);
			start = i + 1;
		}
		i++;
	}
	if (!add_path_element(&args, start, i - start))
		return (NULL);
	args.paths[args.j] = NULL;
	return (args.paths);
}

char	**split_path(t_gc *gc, char *path_str)
{
	char	**paths;
	int		count;

	if (!path_str)
		return (NULL);
	count = count_path_elements(path_str);
	paths = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!paths)
		return (NULL);
	return (process_path_elements(paths, path_str, gc));
}
