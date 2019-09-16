/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:25 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/16 12:29:21 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "client.h"
#include "cbuf.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netdb.h>

int		rpl_welcome(struct s_client *const self,
	struct s_message const *const msg)
{
	(void)self;
	if (msg->params.param[0])
	{
		printf(" * %s\n", msg->params.param[0]);
	}
	return (0);
}

int		client_execute_command(struct s_client *const self,
	struct s_client_msg const *const cmd)
{
	const struct s_tuple_cmds	cmds[] = {
		{ .cmd = CMD_CONNECT, .f = self->connect },
		{ .cmd = CMD_NICK, .f = self->nick },
		{ .cmd = CMD_JOIN, .f = self->join },
		{ .cmd = CMD_MSG, .f = self->message },
		{ .cmd = CMD_USER, .f = self->user },
		{ .cmd = CMD_LEAVE, .f = self->leave },
		{ .cmd = CMD_WHO, .f = self->who },
		{ .cmd = CMD_QUIT, .f = self->quit },
	};
	size_t						ii;

	if (!self)
		return (-1);
	if (!cmd)
		return (-1);
	ii = 0;
	while (ii < sizeof(cmds) / sizeof(*cmds))
	{
		if (cmds[ii].cmd == cmd->cmd)
			cmds[ii].f(self, cmd);
		ii++;
	}
	return (0);
}

int		argconnect(struct s_client *client, char *hoststr, char *portstr)
{
	char				nickname[SNICK];
	int					ret;
	struct s_client_msg	msg;

	write(1, "Nickname? ", 10);
	ret = read(0, nickname, SNICK - 1);
	if (ret < 0)
		return (-1);
	nickname[ret - 1] = 0;
	msg.cmd = CMD_NICK;
	msg.nparam = 1;
	ft_memcpy(msg.params[0], nickname, ret);
	client->nick(client, &msg);
	msg.cmd = CMD_CONNECT;
	msg.nparam = 2;
	ft_strlcpy(msg.params[0], hoststr, SPARAM);
	ft_strlcpy(msg.params[1], portstr, SPARAM);
	return (client->connect(client, &msg));
}

int		main(int ac, char *av[])
{
	struct s_client		client;

	signal(SIGPIPE, SIG_IGN);
	if (client_init(&client) < 0)
		return (EXIT_FAILURE);
	if (ac == 3)
	{
		if (argconnect(&client, av[1], av[2]) < 0)
		{
			client_destroy(&client);
			return (EXIT_FAILURE);
		}
	}
	if (client.run(&client) < 0)
	{
		client_destroy(&client);
		return (EXIT_FAILURE);
	}
	client_destroy(&client);
	return (0);
}
