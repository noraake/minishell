/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:25 by noakebli          #+#    #+#             */
/*   Updated: 2025/07/17 11:43:25 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirections(t_cmd *cmd, t_gc *gc)
{
	int	fd;
	int	flags;

	(void)gc;
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			return (perror("open infile"), 1);
		dup2(fd, 0);
		close(fd);
	}
	if (cmd->outfile)
	{
		if (cmd->append)
			flags = O_APPEND | O_WRONLY | O_CREAT;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(cmd->outfile, flags, 0644);
		if (fd < 0)
			return (perror("open outfile"), 1);
		dup2(fd, 1);
		close(fd);
	}
	return (0);
}
