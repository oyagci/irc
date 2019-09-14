/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:21 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:56:25 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static int			message_cmd(struct s_message *msg, char const *input)
{
	input += msg->cmd.len;
	msg->params = params(input);
	if (msg->params)
		input += msg->params->len;
	if (!(msg->crlf = crlf(input)))
	{
		message_del(msg);
		return (-1);
	}
	return (0);
}

int					message(struct s_message *msg, char const *input)
{
	if (input[0] == ':')
	{
		input += 1;
		if (prefix(&msg->prefix, input) < 0 || input[msg->prefix.len] != ' ')
		{
			message_del(msg);
			return (-1);
		}
		input += msg->prefix.len + 1;
	}
	if (!command(&msg->cmd, input))
		return (message_cmd(msg, input));
	else
		message_del(msg);
	return (0);
}

void				message_del(struct s_message *msg)
{
	if (msg)
	{
		command_del(&msg->cmd);
		params_del(&msg->params);
		crlf_del(&msg->crlf);
	}
}
