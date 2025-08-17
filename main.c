/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:00 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 04:58:21 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**duplicate_environ(t_gc *gc, char **envp)
{
	int		count;
	int		i;
	char	**env_copy;

	if (!envp)
		return (NULL);
	count = 0;
	i = 0;
	while (envp[count])
		count++;
	env_copy = gc_malloc(gc, sizeof(char *) * (count + 1));
	if (!env_copy)
		return (NULL);
	while (i < count)
	{
		env_copy[i] = gc_strdup(gc, envp[i]);
		if (!env_copy[i])
			return (NULL);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

t_env	*init_environment(t_gc *gc, char **envp)
{
	t_env	*env;

	env = gc_malloc(gc, sizeof(t_env));
	if (!env)
		return (NULL);
	env->gc = gc;
	env->env = duplicate_environ(gc, envp);
	if (!env->env)
		return (NULL);
	env->last_status = 0;
	return (env);
}

void	cleanup_readline(void)
{
	clear_history();
	rl_cleanup_after_signal();
	rl_clear_signals();
}

int	main(int argc, char **argv, char **envp)
{
	t_gc	*gc;
	t_env	*env;
	int		exit_status;

	(void)argc;
	(void)argv;
	gc = gc_create();
	if (!gc)
	{
		write(2, "Erreur : impossible de créer le GC\n", 35);
		return (1);
	}
	gc_init(gc);
	env = init_environment(gc, envp);
	if (!env)
	{
		write(2, "Erreur : init env\n", 18);
		gc_destroy(gc);
		return (1);
	}
	loop_minishell(env);
	exit_status = env->last_status;
	cleanup_readline();
	gc_destroy(gc);
	return (exit_status);
}
