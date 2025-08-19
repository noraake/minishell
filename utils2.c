/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 02:59:41 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/19 21:05:53 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_gc(char *s1, char *s2, t_gc *gc)
{
	size_t	len1;
	size_t	len2;
	char	*res;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = gc_malloc(gc, len1 + len2 + 1);
	if (!res)
		return (NULL);
	memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}

char	*ft_strndup_gc(const char *s, size_t n, t_gc *gc)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	if (!s)
		return (NULL);
	while (len < n && s[len])
		len++;
	dup = gc_malloc(gc, (len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

bool	has_quotes(const char *s)
{
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (true);
		s++;
	}
	return (false);
}

void	cleanup_and_exit(t_gc *gc, int status)
{
	if (gc)
		gc_free_all(gc);
	free(gc);
	exit(status);
}

int	has_exec_permission(const char *path)
{
	if (access(path, X_OK) == 0)
		return (1);
	return (0);
}
