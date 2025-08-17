/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 02:54:03 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/12 21:09:17 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_path(char **args, t_env *env)
{
	char	*home;
	char	*oldpwd;

	if (!args[1])
	{
		home = get_env_value(env, "HOME");
		if (!home)
			return (write(2, "cd: HOME not set\n", 18), NULL);
		return (home);
	}
	if (!ft_strcmp(args[1], "-"))
	{
		oldpwd = get_env_value(env, "OLDPWD");
		if (!oldpwd)
			return (write(2, "cd: OLDPWD not set\n", 20), NULL);
		printf("%s\n", oldpwd);
		return (oldpwd);
	}
	return (args[1]);
}

void	update_cd_env(t_env *env, char *oldpwd, char *newpwd, t_gc *gc)
{
	char	*old_var;
	char	*new_var;

	old_var = ft_strjoin_gc("OLDPWD=", oldpwd, gc);
	new_var = ft_strjoin_gc("PWD=", newpwd, gc);
	if (old_var)
		add_or_update_env_var(env, old_var, gc);
	if (new_var)
		add_or_update_env_var(env, new_var, gc);
}

int	builtin_cd(char **args, t_env *env, t_gc *gc)
{
	char	oldpwd[1024];
	char	newpwd[1024];
	char	*path;

	if (args[1] && args[2])
		return (ft_putstr_fd("cd: too many arguments\n", 2), 1);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		return (perror("getcwd"), 1);
	path = get_target_path(args, env);
	if (!path)
		return (1);
	if (chdir(path) == -1 || !getcwd(newpwd, sizeof(newpwd)))
		return (perror("cd"), 1);
	update_cd_env(env, oldpwd, newpwd, gc);
	return (0);
}
