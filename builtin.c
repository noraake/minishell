/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:05 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 23:57:02 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int			i;
	const char	*builtins[] = {"cd", "echo", "pwd", "exit", "export", "unset",
		"env", ".", NULL};

	if (!cmd)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (!ft_strcmp(cmd, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}

int	builtin_pwd(t_env *env)
{
//	char	*cwd;

//	printf("test\n");
	printf("%s\n", get_env_value(env, "PWD"));
	// cwd = getcwd(NULL, 0);
	// if (!cwd)
	// {
	// 	perror("minishell: pwd");
	// 	env->last_status = 1;
	// 	return (1);
	// }
	// printf("%s\n", cwd);
	// free(cwd);
	env->last_status = 0;
	return (0);
}

int	is_numeric(char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	builtin_dot(char **args, t_env *env, t_gc *gc)
{
	(void)gc;
	if (!args[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		env->last_status = 2;
		return (2);
	}
	if (access(args[1], F_OK) != 0)
	{
		ft_putstr_fd("minishell: .: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		env->last_status = 1;
		return (1);
	}
	env->last_status = 0;
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_env *env, t_gc *gc, bool in_child)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, env, gc));
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd(env));
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, gc, in_child));
	if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, env, gc));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, env));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(env));
	if (!ft_strcmp(cmd->args[0], "."))
		return (builtin_dot(cmd->args, env, gc));
	return (1);
}
