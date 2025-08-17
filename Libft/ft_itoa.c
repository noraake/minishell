/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 11:50:39 by noakebli          #+#    #+#             */
/*   Updated: 2024/11/06 15:15:52 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static size_t	ft_numlen(int num)
{
	int		len;

	len = 0;
	if (num <= 0)
	{
		len++;
	}
	while (num != 0)
	{
		num = num / 10;
		len++;
	}
	return (len);
}

static void	ft_itoa_positive(long nb, char *str, int *len)
{
	while (nb > 0)
	{
		str[*len] = nb % 10 + '0';
		(*len)--;
		nb /= 10;
	}
}

static void	ft_handle_zero(char *str, int *len, long nb)
{
	if (nb == 0)
	{
		str[0] = '0';
		*len = 0;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;

	nb = n;
	len = ft_numlen(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	str[len--] = '\0';
	ft_handle_zero(str, &len, nb);
	if (nb != 0)
	{
		ft_itoa_positive(nb, str, &len);
	}
	return (str);
}
