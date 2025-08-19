/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:27:57 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/18 17:45:39 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_numeric_error(char *arg, t_gc *gc)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	gc_free_all(gc);
	free(gc);
	exit(2);
}

static int	check_exit_args(char **args, int argc, t_gc *gc, int *exit_code)
{
	if (!is_numeric(args[1]) || !is_llong(args[1]))
		exit_numeric_error(args[1], gc);
	if (argc > 2)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	*exit_code = (int)((unsigned char)ft_atoi(args[1]));
	return (0);
}

int	builtin_exit(char **args, t_gc *gc, bool in_child)
{
	int		argc;
	int		exit_code;
	t_env	*env;

	argc = 0;
	exit_code = 0;
	env = get_env_for_signals(NULL);
	while (args[argc])
		argc++;
	if (!in_child)
		printf("exit\n");
	if (argc >= 2)
	{
		if (check_exit_args(args, argc, gc, &exit_code))
			return (1);
	}
	else
		exit_code = env->last_status;
	gc_free_all(gc);
	free(gc);
	exit(exit_code);
}
