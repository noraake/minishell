/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_create.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/08/06 21:54:56 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/06 21:54:56 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_redir_type(t_redir *redir, char *op)
{
	if (ft_strcmp(op, "<") == 0)
		redir->type = 0;
	else if (ft_strcmp(op, ">") == 0)
		redir->type = 1;
	else if (ft_strcmp(op, ">>") == 0)
		redir->type = 2;
	else if (ft_strcmp(op, "<<") == 0)
		redir->type = 3;
	else
		redir->type = -1;
	return (redir->type);
}

t_redir	*create_redirection(char *op, char *file, bool quoted, t_gc *gc)
{
	t_redir	*redir;

	redir = gc_malloc(gc, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = gc_strdup(gc, file);
	if (!redir->file)
		return (NULL);
	set_redir_type(redir, op);
	redir->quoted = quoted;
	redir->next = NULL;
	redir->heredoc_fd = -1;
	redir->heredoc_processed = false;
	redir->target_fd = -1;
	return (redir);
}
