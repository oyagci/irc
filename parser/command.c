/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 11:21:16 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <libft.h>
#include <stdlib.h>
#include <stdio.h>

static void				get_cmd(struct s_command *buf, char const *input,
	int i)
{
	while (ft_isalpha(input[i]))
		i += 1;
	buf->data = ft_strndup((char *)input, i);
	buf->len = ft_strlen(buf->data);
}

static struct s_command	*get_num(struct s_command *buf, char const *input,
	int i)
{
	while (ft_isdigit(input[i]))
	{
		if (i >= 3)
		{
			command_del(&buf);
			return (NULL);
		}
		i += 1;
	}
	buf->data = ft_strndup((char *)input, i);
	buf->len = ft_strlen(buf->data);
	return (buf);
}

struct s_command		*command(char const *input)
{
	struct s_command	*cmd;
	int					i;

	cmd = ft_memalloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	i = 0;
	if (ft_isalpha(input[0]))
		get_cmd(cmd, input, i);
	else if (ft_isdigit(input[0]))
		cmd = get_num(cmd, input, i);
	else
	{
		free(cmd);
		cmd = NULL;
	}
	return (cmd);
}

void					command_del(struct s_command **cmd)
{
	if (*cmd)
	{
		free((*cmd)->data);
		free(*cmd);
		*cmd = NULL;
	}
}
