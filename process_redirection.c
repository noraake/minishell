/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:58:47 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 22:13:54 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_in_redir(t_redir *redir, int *last_fd)
{
	int	fd;

	if (*last_fd != -1)
		close(*last_fd);
	fd = open(redir->file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ", 2);
		if (errno == ENOENT)
			ft_putstr_fd("No such file or directory\n", 2);
		else if (errno == EACCES)
			ft_putstr_fd("Permission denied\n", 2);
		else
			perror("");
		return (-1);
	}
	*last_fd = fd;
	return (0);
}

int	handle_out_append_redir(t_redir *redir)
{
	if (redir->type == REDIR_OUT)
		return (handle_output_redir(redir->file));
	else if (redir->type == APPEND)
		return (handle_append_redir(redir->file));
	return (0);
}

int	preprocess_heredocs(t_cmd *cmds, t_env *env, t_gc *gc)
{
	t_cmd	*current;
	t_redir	*redir;
	int		fd;

	current = cmds;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				fd = handle_heredoc(redir->file, !redir->quoted, env, gc);
				if (fd < 0)
					return (-1);
				redir->type = HEREDOC_PROCESSED;
				redir->heredoc_fd = fd;
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

int	handle_heredoc_redir(t_redir *redir, int *last_fd, t_env *env, t_gc *gc)
{
	int	fd;

	if (*last_fd != -1)
		close(*last_fd);
	if (redir->heredoc_processed)
	{
		*last_fd = redir->heredoc_fd;
		return (0);
	}
	fd = handle_heredoc(redir->file, !redir->quoted, env, gc);
	if (fd == -2)
		return (-2);
	if (fd == -1)
		return (-1);
	*last_fd = fd;
	return (0);
}
//int	loop_here

int	handle_redir_entry(t_redir *redir, int *last_fd, t_env *env, t_gc *gc)
{
	if (redir->type == REDIR_IN)
		return (handle_in_redir(redir, last_fd));
	else if (redir->type == HEREDOC_PROCESSED)
	{
		if (*last_fd != -1)
			close(*last_fd);
		*last_fd = redir->heredoc_fd;
		return (0);
	}
	else if (redir->type == REDIR_OUT || redir->type == APPEND)
		return (handle_out_append_redir(redir));
	else if (redir->type == HEREDOC)
		return (handle_heredoc_redir(redir, last_fd, env, gc));
	else if (redir->type == REDIR_FD)
	{
		if (dup2(redir->target_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (-1);
		}
	}
	return (0);
}
