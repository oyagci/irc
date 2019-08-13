/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:21 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/09 11:53:36 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdlib.h>

/*
** Augmented BNF:
** message =  [ ":" prefix SPACE ] command [ params ] crlf
*/
struct s_message	*message(char const *input)
{
	struct s_message	*msg;

	msg = ft_memalloc(sizeof(struct s_message));
	if (!msg)
		return (NULL);
	if (input[0] == ':')
	{
		input += 1;
		msg->prefix = prefix(input);
		if (!msg->prefix || input[msg->prefix->len] != ' ')
		{
			message_del(&msg);
			return (NULL);
		}
		input += msg->prefix->len + 1;
	}

	msg->cmd = command(input);
	if (msg->cmd)
	{
		input += msg->cmd->len;
		msg->params = params(input);
		if (msg->params)
			input += msg->params->len;
		if (!crlf(input))
			message_del(&msg);
	}
	else
		message_del(&msg);
	return (msg);
}

void			message_del(struct s_message **msg)
{
	if (*msg)
	{
		prefix_del(&((*msg)->prefix));
		command_del(&((*msg)->cmd));
		free(*msg);
		*msg = NULL;
	}
}
