/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:05 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/05 14:26:54 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	int			i;
	const char	*builtins[] = {
		"cd", "echo", "pwd", "exit", "export", "unset", "env", NULL
	};

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

int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
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

int	exec_builtin(t_cmd *cmd, t_env *env, t_gc *gc)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, env, gc));
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, gc));
	if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, env, gc));
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, env));
	if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(env));
	return (1);
}
