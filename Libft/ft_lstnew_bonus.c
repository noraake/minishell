/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:37:16 by noakebli          #+#    #+#             */
/*   Updated: 2025/01/10 14:03:36 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*nd;

	nd = (t_list *)malloc(sizeof(*nd));
	if (!nd)
	{
		return (NULL);
	}
	nd->content = content;
	nd->next = NULL;
	return (nd);
}
