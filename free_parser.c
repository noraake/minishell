/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:47 by noakebli          #+#    #+#             */
/*   Updated: 2025/07/17 11:42:47 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirections(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->file);
		free(tmp);
	}
}

void	free_command_table(t_cmd **cmds)
{
	int	i;
	int	j;

	i = 0;
	if (!cmds)
		return ;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i]->args && cmds[i]->args[j])
			free(cmds[i]->args[j++]);
		free(cmds[i]->args);
		free_redirections(cmds[i]->redirections);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
