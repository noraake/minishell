/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:41:56 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 20:42:34 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fd_if_valid(int fd)
{
	if (fd >= 0)
		close(fd);
}

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

static int	exec_with_checks_core(char *path, t_cmd *cmd, t_env *env)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			return (126);
		}
		if (!has_exec_permission(path))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			return (126);
		}
		execve(path, cmd->args, env_to_array(env, env->gc));
		return (perror("execve"), 127);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (127);
}

void	exec_with_checks_exit(char *path, t_cmd *cmd, t_env *env)
{
	int	status;

	status = exec_with_checks_core(path, cmd, env);
	cleanup_and_exit(env->gc, status);
}
