/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/08/12 10:16:56 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "logger.h"

int	nparams(char **params)
{
	int ii;

	ii = 0;
	while (ii < 15 && params[ii] != 0)
		ii++;
	return (ii);
}

int	execute_command(struct s_client *c)
{
	const struct s_irc_cmds	cmds[] = {
		{ .name = "PASS", .f = irc_pass }, { .name = "NICK", .f = irc_nick },
		{ .name = "USER", .f = irc_user }, { .name = "JOIN", .f = irc_join },
		{ .name = "PART", .f = irc_part }, { .name = "WHO", .f = irc_who },
		{ .name = "PRIVMSG", .f = irc_privmsg },
	};
	size_t					ii;
	struct s_message		*msg;
	int						validcmd;

	msg = message(c->buffer.data);
	if (!msg)
		return (0);
	ii = 0;
	validcmd = 0;
	while (ii < sizeof(cmds) / sizeof(struct s_irc_cmds))
	{
		if (ft_strequ(cmds[ii].name, msg->cmd->data))
		{
			validcmd = 1;
			cmds[ii].f(c, msg->params->param, nparams(msg->params->param));
			break ;
		}
		ii++;
	}
	message_del(&msg);
	if (!validcmd)
		c->server->queuecode(c->server, c, ERR_UNKNOWNCOMMAND);
	return (0);
}

int	main(int ac, char *av[])
{
	struct s_server		server;

	if (ac < 2)
	{
		printf("Usage: %s <port>\n", av[0]);
		exit(EXIT_FAILURE);
	}
	server_init(&server, ft_atoi(av[1]));
	server.run(&server);
	return (0);
}
