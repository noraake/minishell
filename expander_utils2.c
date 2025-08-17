/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:14:21 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/15 23:05:01 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_char_in_expand(t_expander_ctx *ctx)
{
	bool	in_single;
	bool	in_double;

	in_single = false;
	in_double = false;
	while (ctx->str[ctx->i])
	{
		if (update_quote_state(ctx->str[ctx->i], &in_single, &in_double))
		{
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
		}
		else if (ctx->str[ctx->i] == '$' && !in_single)
		{
			handle_variable(ctx);
		}
		else
		{
			ctx->result[(ctx->j)++] = ctx->str[(ctx->i)++];
		}
	}
}

bool	update_quote_state(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !*in_double)
	{
		*in_single = !*in_single;
		return (true);
	}
	else if (c == '\"' && !*in_single)
	{
		*in_double = !*in_double;
		return (true);
	}
	return (false);
}

static int	process_var_question_mark(int *i, t_env *env)
{
	char	*val;
	int		size;

	val = ft_itoa(env->last_status);
	size = ft_strlen(val);
	free(val);
	*i += 2;
	return (size);
}

static int	process_var_name_and_size(char *str, int *i, t_env *env)
{
	char	var[256];
	char	*val;
	int		k;
	int		size;

	size = 0;
	(*i)++;
	k = 0;
	while (str[*i] && (ft_isalnum_custom(str[*i]) || str[*i] == '_'))
		var[k++] = str[(*i)++];
	var[k] = '\0';
	val = get_env_value(env, var);
	if (val)
		size += ft_strlen(val);
	return (size);
}

int	process_var_size(char *str, int *i, t_env *env)
{
	if (str[*i + 1] == '?')
		return (process_var_question_mark(i, env));
	else if (ft_isalnum_custom(str[*i + 1]) || str[*i + 1] == '_')
		return (process_var_name_and_size(str, i, env));
	else
	{
		(*i)++;
		return (1);
	}
}
