/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:30 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 21:29:56 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_absolute_or_relative(char *cmd, t_gc *gc)
{
	struct stat	st;

	if (stat(cmd, &st) == 0)
		return (gc_strdup(gc, cmd));
	return (NULL);
}

static char	*search_in_paths(char **paths, char *cmd, t_gc *gc)
{
	int			i;
	char		*full_path;
	struct stat	st;
	char		*found_file;

	i = 0;
	found_file = NULL;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd, gc);
		if (full_path && access(full_path, F_OK) == 0)
		{
			if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
			{
				if (!found_file)
					found_file = full_path;
				if (access(full_path, X_OK) == 0)
					return (full_path);
			}
		}
		i++;
	}
	return (found_file);
}

char	*find_command_path(char *cmd, t_env *env, t_gc *gc)
{
	char	*path_env;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (handle_absolute_or_relative(cmd, gc));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = split_path(gc, path_env);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd, gc));
}

int	handle_append_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		if (errno == EACCES)
			ft_putstr_fd("Permission denied\n", 2);
		else
			perror("");
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		if (errno == ENOENT)
			ft_putstr_fd("No such file or directory\n", 2);
		else if (errno == EACCES)
			ft_putstr_fd("Permission denied\n", 2);
		else
			perror("");
		return (-1);
	}
	return (fd);
}
