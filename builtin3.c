/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:42:14 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/17 01:42:24 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_n_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	safe_write(int fd, const char *s)
{
	size_t	len;
	ssize_t	written;

	if (!s)
		return ;
	len = ft_strlen(s);
	while (len > 0)
	{
		written = write(fd, s, len);
		if (written <= 0)
			break ;
		s += written;
		len -= written;
	}
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	first;

	i = 1;
	newline = 1;
	first = 1;
	while (args[i] && is_valid_n_option(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		if (!first)
			safe_write(STDOUT_FILENO, " ");
		safe_write(STDOUT_FILENO, args[i]);
		first = 0;
		i++;
	}
	if (newline)
		safe_write(STDOUT_FILENO, "\n");
	return (0);
}

static bool	is_valid_identifier(const char *key)
{
	int	i;

	if (!key || !key[0] || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (false);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_unset(char **args, t_env *env)
{
	int	i;

	if (!args || !env)
		return (1);
	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			remove_env_var(env, args[i]);
		i++;
	}
	return (0);
}
