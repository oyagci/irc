/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:21 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/06 16:07:02 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"
#include <stdlib.h>
#include "logger.h"

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
		msglog(LOGDEBUG, msg->prefix->data);
		if (!msg->prefix || input[msg->prefix->len] != ' ')
		{
			msglog(LOGDEBUG, "Invalid prefix");
			message_del(&msg);
			return (NULL);
		}
		input += msg->prefix->len + 1;
	}

	msg->cmd = command(input);
	if (msg->cmd)
	{
		LOG(LOGDEBUG, "Command found: %s", msg->cmd->data);
		input += msg->cmd->len;
		msg->params = params(input);
		if (msg->params)
			input += msg->params->len;
		if (!crlf(input))
		{
			LOG(LOGDEBUG, "Message is not terminated by CR-LF");
			message_del(&msg);
		}
	}
	else
	{
		LOG(LOGDEBUG, "No command found");
		message_del(&msg);
	}
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
