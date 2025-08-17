/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 00:01:22 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/11 10:33:01 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error_invalid(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	print_invalid_identifier_error(const char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	check_numeric_range(const char *str, int sign)
{
	unsigned long long	num;
	unsigned long long	limit;

	num = 0;
	if (sign == 1)
		limit = LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + (*str - '0');
		if (num > limit)
			return (0);
		str++;
	}
	return (*str == '\0');
}

int	lid_llong(const char *str)
{
	int	sign;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!(*str >= '0' && *str <= '9'))
		return (0);
	return (check_numeric_range(str, sign));
}

void	print_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (ft_strchr(cmd, '/'))
		ft_putstr_fd(": No such file or directory\n", 2);
	else
		ft_putstr_fd(": command not found\n", 2);
}
