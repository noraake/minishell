/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expader_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:09:59 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/07 15:10:40 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_status_var(char *result, int *j, t_env *env)
{
	char	*status;
	int		m;

	status = ft_itoa(env->last_status);
	m = 0;
	while (status[m])
		result[(*j)++] = status[m++];
	free(status);
}

void	process_var_name(char *str, int *i, char *var_name)
{
	int	k;

	k = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		var_name[k++] = str[(*i)++];
	var_name[k] = '\0';
}

void	append_var_value(char *result, int *j, char *value)
{
	int	m;

	m = 0;
	while (value[m])
		result[(*j)++] = value[m++];
}

void	handle_variable(t_expander_ctx *ctx)
{
	char	var_name[256];
	char	*value;

	ctx->i++;
	if (ctx->str[ctx->i] == '?')
	{
		handle_status_var(ctx->result, &(ctx->j), ctx->env);
		ctx->i++;
		return ;
	}
	if (!ft_isalpha(ctx->str[ctx->i]) && ctx->str[ctx->i] != '_')
	{
		ctx->result[(ctx->j)++] = '$';
		return ;
	}
	process_var_name(ctx->str, &(ctx->i), var_name);
	value = get_env_value(ctx->env, var_name);
	if (!value)
		return ;
	append_var_value(ctx->result, &(ctx->j), value);
}
