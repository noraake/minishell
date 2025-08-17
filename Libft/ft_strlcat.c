/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 18:31:12 by noakebli          #+#    #+#             */
/*   Updated: 2024/11/07 20:37:35 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	dst_lent;
	unsigned int	src_lent;
	unsigned int	i;
	unsigned int	j;

	if ((!src || !dest) && size == 0)
		return (0);
	dst_lent = ft_strlen(dest);
	src_lent = ft_strlen(src);
	if (size == 0)
	{
		return (src_lent);
	}
	if (size <= dst_lent)
		return (size + src_lent);
	i = dst_lent;
	j = 0;
	while (src[j] && i < (size - 1))
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dst_lent + src_lent);
}
