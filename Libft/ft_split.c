/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:59:19 by noakebli          #+#    #+#             */
/*   Updated: 2024/11/15 16:59:22 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_word(const char *str, char c)
{
	int		i;
	int		word;
	int		count;

	i = 0;
	word = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != c && word == 0)
		{
			word = 1;
			count++;
		}
		else if (str[i] == c)
		{
			word = 0;
		}
		i++;
	}
	return (count);
}

static int	len_word(const char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			break ;
		i++;
	}
	return (i);
}

static char	*allocate_word(const char *str, int len)
{
	char	*mot;
	int		i;

	i = 0;
	mot = malloc(sizeof(char) * (len + 1));
	if (!mot)
	{
		return (NULL);
	}
	while (str[i] && i < len)
	{
		mot[i] = str[i];
		i++;
	}
	mot[i] = '\0';
	return (mot);
}

static void	*ft_free(char *res)
{
	int		i;

	i = 0;
	while (res[i])
	{
		free(res);
		i++;
	}
	free (res);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		lenmot;
	char	**memmot;
	int		countmots;
	int		j;

	j = 0;
	if (!s)
		return (NULL);
	countmots = count_word(s, c);
	memmot = (char **)malloc(sizeof(char *) * (countmots + 1));
	if (!memmot)
		return (NULL);
	while (s && j < countmots)
	{
		while (*s && *s == c)
			s++;
		lenmot = len_word(s, c);
		memmot[j] = allocate_word(s, lenmot);
		if (!memmot)
			return (ft_free(*memmot));
		s = s + lenmot;
		j++;
	}
	memmot[j] = NULL;
	return (memmot);
}

// int main()
// {
// 	const char s[] = "|noura|ikram|fati|";
// 	char **str;
// 	int	i;

// 	str = ft_split(s, '|');
// 	i = 0;
// 	while (str[i])
// 	{
// 		printf("%s\n", str[i]);
// 		i++;
// 	}
// }
