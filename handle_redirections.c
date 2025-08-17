/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 15:59:49 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 03:12:28 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_error(int *last_fd, t_env *env, t_gc *gc,
		int original_status)
{
	if (*last_fd != -1)
		close(*last_fd);
	if (env->last_status != original_status)
	{
		cleanup_and_exit(gc, env->last_status);
		return (env->last_status);
	}
	env->last_status = 1;
	cleanup_and_exit(gc, 1);
	return (1);
}

static int	process_redirection(t_redir *redir, int *last_fd, t_env *env,
		t_gc *gc)
{
	int	ret;
	int	original_status;

	original_status = env->last_status;
	ret = handle_redir_entry(redir, last_fd, env, gc);
	if (ret == -2)
	{
		if (*last_fd != -1)
			close(*last_fd);
		return (130);
	}
	if (ret == -1)
		return (handle_redir_error(last_fd, env, gc, original_status));
	return (0);
}

int	handle_redirections_new(t_cmd *cmd, t_env *env, t_gc *gc)
{
	t_redir	*redir;
	int		last_fd;
	int		ret;

	last_fd = -1;
	redir = cmd->redirections;
	while (redir)
	{
		ret = process_redirection(redir, &last_fd, env, gc);
		if (ret)
			return (ret);
		redir = redir->next;
	}
	if (last_fd != -1)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
	return (0);
}
