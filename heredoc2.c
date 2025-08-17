/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:03:10 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 16:23:15 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_gc	*get_gc(t_gc *new_gc)
{
	static t_gc	*gc = NULL;

	if (new_gc)
		gc = new_gc;
	return (gc);
}

void	heredoc_sigint_handler(int sig)
{
	t_gc	*gc;

	(void)sig;
	gc = get_gc(NULL);
	if (gc)
		gc_free_all(gc);
	exit(130);
}

void	handle(int sig)
{
	cleanup_and_exit(get_gc(NULL), sig + 128);
}
