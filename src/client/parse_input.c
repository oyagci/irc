/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 13:04:49 by oyagci            #+#    #+#             */
/*   Updated: 2019/11/25 12:21:03 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include <stdio.h>
#include "libft.h"
#include <stdlib.h>

void	set_command(char const **inputp, struct s_client_msg *buf)
{
	const struct s_client_cmds	cmds[] = {
		{ "nick", CMD_NICK, 1 }, { "join", CMD_JOIN, 1 }, { "who", CMD_WHO, 0 },
		{ "leave", CMD_LEAVE, 1 }, { "msg", CMD_MSG, 2 },
		{ "quit", CMD_QUIT, 1 }, { "connect", CMD_CONNECT, 2 }
	};
	char const					*input;
	int							ii;
	size_t						jj;

	input = *inputp;
	ii = 0;
	while (input[ii] && input[ii] != ' ')
		ii += 1;
	jj = 0;
	while (jj < sizeof(cmds) / sizeof(*cmds))
	{
		if (ft_strnequ(input, cmds[jj].str,
				max(ft_strlen(cmds[jj].str), ii - 1)))
		{
			*inputp += ii;
			buf->cmd = cmds[jj].cmd;
			buf->nparam = cmds[jj].nparam;
		}
		jj += 1;
	}
}

void	set_param(char *buf, char const *input,
	size_t start, size_t end)
{
	char	tmp[255];

	ft_memset(tmp, 0, sizeof(tmp));
	ft_strncpy(tmp, input + start, end - start);
	ft_strlcpy(buf, tmp, 255);
}

size_t	set_params(char const *input, struct s_client_msg *buf)
{
	int		start;
	int		end;
	size_t	ii;

	start = 0;
	end = 0;
	ii = 0;
	if (buf->nparam > 0)
	{
		while (input[end] != '\0' && ii < buf->nparam - 1)
		{
			end = start;
			while (input[end] != ' ' && input[end] != '\0')
				end++;
			set_param(buf->params[ii], input, start, end);
			if (input[end] == ' ')
				end++;
			start = end;
			ii += 1;
		}
		if (input[start] != 0)
			ft_strlcpy(buf->params[ii++], input + start, 255);
	}
	return (ii);
}

int		parse_cmd(struct s_client_msg **buf, char const *input)
{
	input += 1;
	set_command(&input, *buf);
	if (*input == ' ')
		input++;
	if (set_params(input, *buf) < (*buf)->nparam)
	{
		ft_putstr("Not enough parameters given (expected");
		ft_putnbr((*buf)->nparam);
		ft_putendl(")");
		return (-1);
	}
	return (0);
}

int		parse_input(struct s_client *const self,
	struct s_client_msg *msg, char const *input)
{
	ft_memset(msg, 0, sizeof(*msg));
	msg->cmd = CMD_NONE;
	if (input[0] == '/')
		return (parse_cmd(&msg, input));
	else if (self->channel)
	{
		msg->cmd = CMD_MSG;
		msg->nparam = 2;
		ft_strlcpy(msg->params[0], self->channel->name, 255);
		ft_strlcpy(msg->params[1], input, 255);
	}
	else
	{
		ft_putendl(" - Not in a channel (/join <channel>)");
		return (-1);
	}
	return (0);
}
