/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oyagci <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/01 10:08:24 by oyagci            #+#    #+#             */
/*   Updated: 2019/09/09 14:27:41 by oyagci           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "logger.h"
#include "libft.h"

int			nparams(char **params)
{
	int ii;

	ii = 0;
	while (ii < 15 && params[ii] != 0)
		ii++;
	return (ii);
}

static void	set_cmds(struct s_irc_cmds cmds[9])
{
	cmds[0].name = "PASS";
	cmds[0].f = irc_pass;
	cmds[1].name = "NICK";
	cmds[1].f = irc_nick;
	cmds[2].name = "USER";
	cmds[2].f = irc_user;
	cmds[3].name = "JOIN";
	cmds[3].f = irc_join;
	cmds[4].name = "PART";
	cmds[4].f = irc_part;
	cmds[5].name = "WHO";
	cmds[5].f = irc_who;
	cmds[6].name = "PONG";
	cmds[6].f = irc_pong;
	cmds[7].name = "QUIT";
	cmds[7].f = irc_quit;
	cmds[8].name = "PRIVMSG";
	cmds[8].f = irc_privmsg;
}

int			execute_command(struct s_client *c, char const *const cmd)
{
	struct s_irc_cmds	cmds[9];
	size_t				ii;
	struct s_message	*msg;
	int					validcmd;
	int					err;

	set_cmds(cmds);
	if (!(msg = message(cmd)))
		return (0);
	validcmd = 0;
	err = 0;
	ii = 0;
	while (ii < sizeof(cmds) / sizeof(struct s_irc_cmds))
	{
		if (ft_strequ(cmds[ii].name, msg->cmd->data))
		{
			validcmd = 1;
			err = cmds[ii].f(c, msg->params->param,
				nparams(msg->params->param));
		}
		ii++;
	}
	message_del(&msg);
	err ? 0 : c->server->queuecode(c->server, c, err);
	return (0);
}

int			main(int ac, char *av[])
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
