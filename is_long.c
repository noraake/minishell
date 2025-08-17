/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochachi <ochachi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 00:40:36 by ochachi           #+#    #+#             */
/*   Updated: 2025/08/07 00:40:46 by ochachi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_whitespace_and_sign(const char *str, int *sign)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

static int	is_within_llong(const char *str, int i, int sign)
{
	unsigned long long	num;
	unsigned long long	max;
	int					digits;

	num = 0;
	digits = 0;
	max = (unsigned long long)LLONG_MAX;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		digits++;
		if ((sign == 1 && num > max)
			|| (sign == -1 && num > (unsigned long long)LLONG_MAX + 1ULL))
			return (0);
		i++;
	}
	if (str[i] != '\0' || digits == 0)
		return (0);
	return (1);
}

int	is_llong(const char *str)
{
	int	sign;
	int	i;

	sign = 1;
	i = skip_whitespace_and_sign(str, &sign);
	return (is_within_llong(str, i, sign));
}
