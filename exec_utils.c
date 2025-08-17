/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:30 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/05 15:49:19 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_full_path(char *dir, char *cmd, t_gc *gc)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin_gc(dir, "/", gc);
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin_gc(tmp, cmd, gc);
	return (full_path);
}

static char	*handle_absolute_or_relative(char *cmd, t_gc *gc)
{
	struct stat	st;

	if (stat(cmd, &st) == 0)
		return (gc_strdup(gc, cmd));
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env, t_gc *gc)
{
	int			i;
	char		*path_env;
	char		**paths;
	char		*full_path;
	struct stat	st;

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
	i = 0;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd, gc);
		if (full_path && stat(full_path, &st) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
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
