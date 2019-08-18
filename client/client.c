/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/08 15:56:24 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "client.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netdb.h>

int	rpl_welcome(struct s_client *const self, struct s_message const *const msg)
{
	(void)self;
	if (msg->params->param[0])
	{
		printf(" * %s\n", msg->params->param[0]);
	}
	return (0);
}

int		client_execute_command(struct s_client *const self,
							   struct s_client_msg const *const cmd)
{
	const struct s_tuple_cmds cmds[] = {
		{ .cmd = CMD_CONNECT, .f = self->connect },
		{ .cmd = CMD_NICK, .f = self->nick },
		{ .cmd = CMD_JOIN, .f = self->join },
		{ .cmd = CMD_MSG, .f = self->message },
		{ .cmd = CMD_USER, .f = self->user },
		{ .cmd = CMD_LEAVE, .f = self->leave },
		{ .cmd = CMD_WHO, .f = self->who },
		{ .cmd = CMD_QUIT, .f = self->quit },
	};
	size_t					ii;

	if (!self)
	{
		LOG(LOGWARN, "%s: self == NULL", __FUNCTION__);
		return (-1);
	}
	if (!cmd)
	{
		LOG(LOGWARN, "%s: msg == NULL", __FUNCTION__);
		return (-1);
	}
	ii = 0;
	while (ii < sizeof(cmds) / sizeof(*cmds))
	{
		if (cmds[ii].cmd == cmd->cmd)
			cmds[ii].f(self, cmd);
		ii++;
	}
	return (0);
};

int	main(void)
{
	struct s_client		client;

	if (client_init(&client) < 0)
	{
		LOG(LOGERR, "Could not initialize client!");
		return (EXIT_FAILURE);
	}
	if (client.run(&client) < 0)
	{
		return (EXIT_FAILURE);
	}
	return (0);
}
