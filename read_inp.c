/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_inp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:18 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/05 02:12:59 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_input(t_gc *gc)
{
	char	*line;
	char	*gc_line;

	line = readline("minishell$ ");
	if (!line)
	{
		write(1, "exit\n", 5);
		return (NULL);
	}
	if (*line)
		add_history(line);
	gc_line = gc_strdup(gc, line);
	free(line);
	return (gc_line);
}
