/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:10:29 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 02:21:54 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_output_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

static int	save_std_fds(int *stdin_fd, int *stdout_fd)
{
	*stdin_fd = dup(STDIN_FILENO);
	*stdout_fd = dup(STDOUT_FILENO);
	if (*stdin_fd == -1 || *stdout_fd == -1)
	{
		perror("dup");
		return (1);
	}
	return (0);
}

static int	restore_std_fds(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
	return (0);
}

static int	handle_redir_and_return(t_cmd *cmd, t_env *env,
				t_gc *gc, int *fds)
{
	int	status;

	status = handle_redirections_new(cmd, env, gc);
	if (status == 130)
	{
		write(STDOUT_FILENO, "\n", 1);
		restore_std_fds(fds[0], fds[1]);
		return (130);
	}
	if (status != 0)
	{
		restore_std_fds(fds[0], fds[1]);
		return (status);
	}
	return (0);
}

int	execute_builtin_with_redirection(t_cmd *cmd, t_env *env, t_gc *gc)
{
	int	fds[2];
	int	ret;
	int	status;

	if (save_std_fds(&fds[0], &fds[1]))
		return (1);
	// if (isatty(STDIN_FILENO))
		// ft_putstr_fd("exit\n", STDERR_FILENO);
	status = handle_redir_and_return(cmd, env, gc, fds);
	if (status != 0)
		return (status);
	ret = exec_builtin(cmd, env, gc);
	restore_std_fds(fds[0], fds[1]);
	return (ret);
}
