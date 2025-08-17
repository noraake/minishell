/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:55 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/06 21:54:03 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_isalnum_custom(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || (c == '_'));
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*result;
	size_t	i;

	if (!s)
		return (NULL);
	result = malloc(n + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

int	is_redirection_string(char *arg)
{
	return (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">") || !ft_strcmp(arg,
			">>") || !ft_strcmp(arg, "<<"));
}
