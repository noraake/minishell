/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noakebli <noakebli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:43:22 by noakebli          #+#    #+#             */
/*   Updated: 2025/08/15 18:26:32 by noakebli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_print_ambiguous(t_redir_ctx *ctx, char *expanded, int i)
{
	return (!expanded || (strchr(expanded, '*') && !ctx->was_quoted[i + 1]));
}

static char	*get_redirection_target(t_redir_ctx *ctx, int *i, bool *quoted)
{
	char	*expanded;

	*quoted = is_quoted(ctx->args[*i + 1]);
	if (ft_strcmp(ctx->args[*i], "<<") == 0)
		return (gc_strdup(ctx->gc, ctx->args[*i + 1]));
	expanded = expand_variables(ctx->args[*i + 1], ctx->env, ctx->gc);
	if (should_print_ambiguous(ctx, expanded, *i))
	{
		if (expanded && strchr(expanded, '*'))
			print_ambiguous_redirect(expanded);
		*ctx->error = 1;
		return (NULL);
	}
	return (expanded);
}

static t_redir	*handle_std_redirection(t_redir_ctx *ctx, int *i)
{
	t_redir	*new;
	char	*final_file;
	bool	quoted;

	if (!ctx->args[*i + 1])
	{
		*ctx->error = print_redirection_error("newline");
		return (NULL);
	}
	final_file = get_redirection_target(ctx, i, &quoted);
	if (!final_file)
		return (NULL);
	new = create_redirection(ctx->args[*i], final_file, quoted, ctx->gc);
	*i += 2;
	return (new);
}

static t_redir	*process_redirection(t_redir_ctx *ctx, int *i)
{
	t_redir	*new;

	if (ft_strncmp(ctx->args[*i], ">&", 2) == 0 && ctx->args[*i][2]
		&& ctx->args[*i][3] == '\0' && ctx->args[*i][2] >= '0'
		&& ctx->args[*i][2] <= '9')
	{
		new = handle_fd_redirection(ctx->args, i, ctx->gc, ctx->error);
	}
	else if (is_redirection(ctx->args[*i]))
	{
		new = handle_std_redirection(ctx, i);
	}
	else
	{
		(*i)++;
		return (NULL);
	}
	return (new);
}

t_redir	*parse_redirections(t_redir_ctx *ctx)
{
	t_redir	*head;
	t_redir	*last;
	t_redir	*new;
	int		i;

	head = NULL;
	last = NULL;
	i = 0;
	while (ctx->args[i] && !*ctx->error)
	{
		new = process_redirection(ctx, &i);
		if (!new && ctx->args[i - 1] && !is_redirection(ctx->args[i - 1]))
			continue ;
		if (!new || *ctx->error)
			return (head);
		if (!head)
			head = new;
		else
			last->next = new;
		last = new;
	}
	if (!has_command(ctx->args))
		*ctx->error = 1;
	return (head);
}
