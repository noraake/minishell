/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 18:03:01 by noakebli          #+#    #+#             */
/*   Updated: 2024/10/28 17:31:31 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	char	*result;
	size_t	i;

	i = 0;
	result = (char *)s;
	while (i < n)
	{
		result[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
