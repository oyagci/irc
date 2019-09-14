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

static int				set_cmd(struct s_command *buf, char const *input)
{
	int	i;

	i = 0;
	while (ft_isalpha(input[i]))
		i += 1;
	if (i == 0)
		return (-1);
	buf->data = ft_strndup((char *)input, i);
	buf->len = ft_strlen(buf->data);
	return (0);
}

static int				set_num(struct s_command *buf, char const *input)
{
	int	i;

	i = 0;
	while (ft_isdigit(input[i]))
	{
		if (i >= 3)
		{
			command_del(buf);
			return (-1);
		}
		i += 1;
	}
	buf->data = ft_strndup((char *)input, i);
	buf->len = ft_strlen(buf->data);
	return (0);
}

int						command(struct s_command *const cmd, char const *input)
{
	if (ft_isalpha(*input))
		set_cmd(cmd, input);
	else if (ft_isdigit(*input))
		set_num(cmd, input);
	else
		return (-1);
	return (0);
}

void					command_del(struct s_command *cmd)
{
	if (cmd)
		free(cmd->data);
}
